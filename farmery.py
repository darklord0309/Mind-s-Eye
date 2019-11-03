import tensorflow as tf
from tensorflow import keras
from tensorflow.keras.models import load_model
import numpy as np
import cv2
from tensorflow.keras.preprocessing.image import img_to_array

model = load_model('final-epoch10.h5')

from google.cloud import storage
from time import sleep
storage_client = storage.Client.from_service_account_json(
        'c:/Users/KIIT/Downloads/NIT.json')
buckets = list(storage_client.list_buckets())
print(buckets)
# https://console.cloud.google.com/storage/browser/[bucket-id]/
bucket = storage_client.get_bucket('nitrfarmery.appspot.com')
# Then do other things...
zebraBlob = bucket.blob('Example1.jpg')
zebraBlob.download_to_filename('D:/python progs/Machine Learning A-Z Template Folder/Part 8 - Deep Learning/Section 40 - Convolutional Neural Networks (CNN)/Example1.jpg')
sleep(15)

image = cv2.imread('Example1.jpg')
image = cv2.resize(image, (210, 210))
image = image.astype("float") / 255.0
image = img_to_array(image)
image = np.expand_dims(image, axis=0)

pred = model.predict(image)

print(pred)

pred = pred.argmax(axis=1)[0]

text = ''
if pred==0 or pred==1 or pred==3:
   text = "plant is prone to pests or diseased."
else:
   text = "Plant is healthy."

print(text)
   
image = cv2.imread('Example1.jpg')

# Write some Text

font                   = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10,500)
fontScale              = 1
fontColor              = (255,255,255)
lineType               = 2

cv2.putText(image,text, 
    bottomLeftCornerOfText, 
    font, 
    fontScale,
    fontColor,
    lineType)

#Display the image
cv2.imshow("img",image)
cv2.waitKey(0)
cv2.imwrite('Example1.jpg',image)

zebraBlob.upload_from_filename(filename='D:/python progs/Machine Learning A-Z Template Folder/Part 8 - Deep Learning/Section 40 - Convolutional Neural Networks (CNN)/Example1.jpg')
