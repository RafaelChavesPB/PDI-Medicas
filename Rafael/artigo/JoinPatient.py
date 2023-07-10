import pydicom
import os

path = 'dicom-files/ok'
pat = {}

for file in os.listdir(path):
    slice = pydicom.dcmread(os.path.join(path, file))
    patHash = slice['0x00100020'].value
    if patHash in pat:
        pat[patHash].append(slice)
    else:
        pat[patHash] = [slice]

for id in pat:
    print(id, len(pat[id]))