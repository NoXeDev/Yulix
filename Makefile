NAME		:=	$(notdir $(CURDIR))

SECTIONS	:=	arm9fw arm11fw

.PHONY:	all clean $(SECTIONS)

all:		boot.firm

clean:
	@$(foreach dir, $(SECTIONS), $(MAKE) -C $(dir) clean &&) true
	@rm -rf *.firm

$(SECTIONS):
	@$(MAKE) -C $@ all

boot.firm:	$(SECTIONS)
	@firmtool build $@ -D arm11fw/arm11fw.elf arm9fw/arm9fw.elf \
	-A 0x18180000 -C XDMA NDMA
	@echo built... $(notdir $@)