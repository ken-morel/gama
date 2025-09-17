package gama

import (
	"image"
	"image/color"
	"image/png"
	"os"

	ico "github.com/Kodeworks/golang-image-ico"
)

func ensureIcon() error {
	if _, err := os.Stat("logo.ico"); err == nil {
		return nil
	}

	if _, err := os.Stat("logo.png"); err != nil {
		if err := createPngLogo(); err != nil {
			return err
		}
	}
	return copyLogoToIco()
}

type GamaColor int

func (GamaColor) RGBA() (uint32, uint32, uint32, uint32) {
	return 170, 120, 170, 255
}

const gamaColor GamaColor = 0

func _drawRectangle(i *image.RGBA, bx, ex, by, ey int, c color.Color) {
	for x := range ex - bx {
		for y := range ey - by {
			i.Set(bx+x, by+y, c)
		}
	}
}

func createLogo() *image.RGBA {
	img := image.NewRGBA(image.Rect(0, 0, 100, 100))
	_drawRectangle(img, 30, 50, 10, 80, gamaColor)
	_drawRectangle(img, 30, 80, 10, 20, gamaColor)
	return img
}

func createPngLogo() error {
	logo := createLogo()

	file, err := os.Create("logo.png")
	if err != nil {
		return err
	}
	defer file.Close()
	return png.Encode(file, logo)
}

func copyLogoToIco() error {
	file, err := os.Open("logo.png")
	if err != nil {
		return err
	}
	defer file.Close()
	image, err := png.Decode(file)
	if err != nil {
		return err
	}
	outFile, err := os.Create("logo.ico")
	if err != nil {
		return err
	}
	defer outFile.Close()
	return ico.Encode(outFile, image)
}
