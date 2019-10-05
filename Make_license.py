from PIL import Image, ImageDraw, ImageFont
import cv2, os, glob
from sys import argv

# 라이센스 이미지를 생성
# 사진 캡쳐 및 임시저장
cap = cv2.VideoCapture(0)
_, frame = cap.read()
cv2.imwrite("cam.png", cv2.resize(frame, (250, 250)))

# 새로운 이미지를 생성하고 캡쳐한 사진을 불러옴
image = Image.new("RGB", (500, 300), (200, 200, 200))
cam = Image.open("cam.png")
image.paste(cam, (25, 25, 275, 275))

# 폰트 (나눔고딕Bold) 불러옴
Fonts = "C:\Windows\Fonts"
FontA = ImageFont.truetype(os.path.join(Fonts, "NanumGothicBold.ttf"), 25)
FontB = ImageFont.truetype(os.path.join(Fonts, "ENBRUSH.TTF"), 40)

# 이미지에 상세 성적 쓰고 저장
draw = ImageDraw.Draw(image)
draw.text((300, 100), argv[1] +"\n" + argv[2], fill = "black", font = FontA)
draw.text((320, 200), "Great!!", fill = "blue", font = FontB)
image.save("result.png")

#임시저장 이미지 삭제
os.remove("cam.png")


