SUBDIRS = tools src/Outlines
     
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

