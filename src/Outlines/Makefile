
GEN=../../bin/gen

default: Roboto-Light.h MaterialIcons-Regular.h
	ls -la

Roboto-Light.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -nroboto::light -o $@ $<

MaterialIcons-Regular.h: MaterialIcons-Regular.ttf $(GEN)
	$(GEN) -wH -xF -iU -nmaterialicons::regular -o $@ $<

