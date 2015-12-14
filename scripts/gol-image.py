import Image

white = (0,0,0)
black = (255,255,255)
data = []

lines = [line.rstrip('\n') for line in open('input.txt')]
height = len(lines)
for line in lines:
	pixels = line.split(' ')
	try:
		width
	except NameError:
		width = len(pixels)
	for pixel in pixels:
		if pixel == '1':
			data.append(black)
		else:
			data.append(white)

im = Image.new('RGB', (width, height))
im.putdata(data)
im.save('output.png')