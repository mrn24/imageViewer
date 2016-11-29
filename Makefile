all: imageViewer.c
	gcc imageViewer.c -o imageViewer -lm

clean:
	rm -rf imageViewer.*~
