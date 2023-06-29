from collections import deque
from .plotting import *
import numpy as np
import cv2

BEGIN_USEFUL_WINDOW, END_USEFUL_WINDOW = 944, 1200
BEGIN_BONE_WINDOW, END_BONE_WINDOW = 220, 256
BEGIN_VENTRICLE_WINDOW, END_VENTRICLE_WINDOW = 70, 100

DIR = [(i, j) for i in (-1, 0, 1) for j in (-1, 0, 1) if i or j]
WHITE = 255
BLACK = 0

def applyWindow(src, start, end=None, stick=True, dtype=None):
    if dtype is None:
        dtype = src.dtype
    currWindowWidth = np.iinfo(dtype).max + 1
    if end is None:
        end = np.max(src)
    newWindowWidth = end - start + 1
    widthMultiplier = currWindowWidth/newWindowWidth
    dst = src.astype('float128')
    dst[dst < start] = start
    dst[dst > end] = end if stick else start
    dst -= start
    dst *= widthMultiplier
    return dst.astype(src.dtype)


def applyBandpassFilter(src, start, end=None, plot=False):
    if end is None:
        end = np.max(src)
    whitePixelValue = np.iinfo(src.dtype).max
    dst = src.copy()
    dst[(src < start) | (src > end)] = 0
    dst[(start <= src) & (src <= end)] = whitePixelValue
    if plot:
        compareTwoImages(src, dst)
    return dst


def changeImgBitDepth(src, currDepth=12):
    if currDepth is None:
        currWindowWidth = np.iinfo(src.dtype).max+1
    else:
        currWindowWidth = 1 << currDepth
    newDepth = 8
    newWindowWidth = 1 << newDepth
    widthMultiplier = newWindowWidth/currWindowWidth
    dst = src.astype(np.float128)
    def valueAdjust(value): return np.uint8(value*widthMultiplier)
    dst = valueAdjust(dst).astype(np.uint8)
    return dst


def getElement(element=cv2.MORPH_RECT, size=(3, 3)):
    return cv2.getStructuringElement(element, size)


def countGroupMembers(startR, startC, mark, img, groupMarker):
    counter = 0
    queue = deque()
    queue.append((startR, startC))
    while (len(queue)):
        r, c = queue.popleft()
        for dr, dc in DIR:
            nr = dr + r
            nc = dc + c
            if nr < 0 or img.shape[0] <= nr:
                continue
            if nc < 0 or img.shape[1] <= nc:
                continue
            if groupMarker[nr][nc] != 0 or img[nr][nc] != WHITE:
                continue
            queue.append((nr, nc))
            groupMarker[nr][nc] = mark
            counter += 1
    return counter


def removeSmallGroups(mask, threshhold=50):
    newMask = mask.copy()
    groupMarker = np.zeros(mask.shape)
    mark = 1
    for R in range(mask.shape[0]):
        for C in range(mask.shape[1]):
            if groupMarker[R][C] != 0 or mask[R][C] != WHITE:
                continue
            members = countGroupMembers(R, C, mark, mask, groupMarker)
            if members < threshhold:
                newMask[groupMarker == mark] = 0
            mark += 1
    return newMask

def fillGroup(startR, startC, startInterval, endInterval, img, mask, visited):
    queue = deque()
    queue.append((startR, startC))
    while (len(queue)):
        r, c = queue.popleft()
        for dr, dc in DIR:
            nr = dr + r
            nc = dc + c
            if nr < 0 or img.shape[0] <= nr:
                continue
            if nc < 0 or img.shape[1] <= nc:
                continue
            if visited[nr][nc] != 0:
                continue
            if img[nc][nc] < startInterval or endInterval < img[nr][nc]:
                continue
            queue.append((nr, nc))
            mask[nr][nc] = WHITE
            visited[nr][nc] = 1


def floodFill(startInterval, endInterval, img, mask):
    newMask = mask.copy()
    visited = np.zeros(newMask.shape)
    for R in range(newMask.shape[0]):
        for C in range(newMask.shape[1]):
            if visited[R][C] != 0 or newMask[R][C] != WHITE:
                continue
            fillGroup(R, C, startInterval, endInterval, img, newMask, visited)
    return newMask

def drawContour(img, mask):
    edges =  cv2.Canny(mask, 85, 200)
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.INTERSECT_FULL)
    contour_image = cv2.drawContours(img, contours, -1, (0, 255, 0), 2)
    return contour_image

def identifyVentricle(original):
    boneMask = applyBandpassFilter(original, BEGIN_BONE_WINDOW, END_BONE_WINDOW)
    maskToRemoveBone = cv2.erode(cv2.bitwise_not(boneMask), getElement(cv2.MORPH_RECT), iterations=40)
    rawVentricleMask = applyBandpassFilter(original, BEGIN_VENTRICLE_WINDOW, END_VENTRICLE_WINDOW)
    rawVentricleMaskWithoutBone = cv2.bitwise_and(rawVentricleMask, maskToRemoveBone)
    areaOfInterest = cv2.erode(rawVentricleMaskWithoutBone, getElement(cv2.MORPH_RECT, (3,3)), iterations=3)
    areaOfInterest = cv2.dilate(areaOfInterest, getElement(cv2.MORPH_CROSS, (7,7)), iterations=20)
    ventricleMask = cv2.bitwise_and(rawVentricleMaskWithoutBone, areaOfInterest)
    ventricleMask = floodFill(BEGIN_VENTRICLE_WINDOW, END_VENTRICLE_WINDOW, original, ventricleMask)
    ventricleMask = removeSmallGroups(ventricleMask, 100)
    return ventricleMask