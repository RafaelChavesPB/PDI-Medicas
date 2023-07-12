import keras
import tensorflow as tf
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
from keras.preprocessing.image import ImageDataGenerator

# gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.4)
# session = tf.Session(config=tf.ConfigProto(gpu_options=gpu_options))

# Define the paths to your image repositories
train_path = './database/train'
test_path = './database/test'

# Define the image size and number of classes
img_size = (512, 512)
img_shape = (*img_size, 3)
num_classes = 2
epochs = 2

# Create an ImageDataGenerator for data loading and preprocessing
datagen = ImageDataGenerator(rescale=1./255)

# Load and preprocess the training data
train_generator = datagen.flow_from_directory(
    train_path,
    target_size=img_size,
    batch_size=8,
    class_mode='categorical',
    shuffle=True
)


# Load and preprocess the testing data
test_generator = datagen.flow_from_directory(
    test_path,
    target_size=img_size,
    batch_size=8,
    class_mode='categorical',
    shuffle=True
)

# Define the model
model = Sequential()
model.add(Conv2D(32, (3, 3), activation='relu', input_shape=img_shape))
model.add(MaxPooling2D(pool_size=(2, 2)))

model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))

model.add(Conv2D(128, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))

# Flatten the output from previous layers
model.add(Flatten())

# Add a fully connected layer
model.add(Dense(128, activation='relu'))

# Add the output layer with softmax activation for classification
model.add(Dense(num_classes, activation='softmax'))

# Compile the model
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy', keras.metrics.Recall()])

# Train the mode
model.fit(train_generator, epochs=epochs)

# Evaluate the model
print(model.evaluate(test_generator))
loss, accuracy, sensitivity = model.evaluate(test_generator)
print('Test Loss:', loss)
print('Test Accuracy:', accuracy)
print('Test Sensivity:', sensitivity)
