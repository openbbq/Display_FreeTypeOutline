
GEN=../../tools/gen

default: Roboto-Light.h MaterialIcons-Regular.h
	ls -la

Roboto-Light.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -nroboto::light -o $@ $<

Roboto-Light.fto: Roboto-Light.ttf $(GEN)
	$(GEN) -wB -a -o $@ $<

MaterialIcons-Regular.h: MaterialIcons-Regular.ttf $(GEN)
	$(GEN) -wH -nmaterialicons::regular -o $@ $<

MaterialIcons-Regular.fto: MaterialIcons-Regular.ttf $(GEN)
	$(GEN) -wB -o $@ $<
