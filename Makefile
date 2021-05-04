CC          := g++
 
TARGET      := main
 
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o
 
CFLAGS      := -std=c++2a -D_DEBUG -Wall -g 
LFLAGS		:= -L/home/teagan/git/PhysX/physx/bin/linux.clang/release/
LIB         := -lm -ldl -lpthread -lSDL2 -lGL -lGLEW -lPhysXCharacterKinematic_static_64 -lPhysXExtensions_static_64 -lPhysX_static_64 -lPhysXPvdSDK_static_64 -lPhysXVehicle_static_64 -lPhysXCooking_static_64 -lPhysXCommon_static_64 -lPhysXFoundation_static_64   
INC         := -I$(INCDIR)/ -I$(SRCDIR)/ -I/home/teagan/git/PhysX/physx/include/ -I/home/teagan/git/PhysX/pxshared/include/
INCDEP      := -I$(INCDIR) -I$(SRCDIR)/ -I/home/teagan/git/PhysX/physx/include/ -I/home/teagan/git/PhysX/pxshared/include/


SOURCES     := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
 
all: resources $(TARGET)
 
remake: cleaner all
 
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/ || :
 
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)
 
clean:
	@$(RM) -rf $(BUILDDIR)
 
cleaner: clean
	@$(RM) -rf $(TARGETDIR)
 
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))
 
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LFLAGS) $(LIB)
 
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp
 
run:
	cd $(TARGETDIR); ./$(TARGET)

.PHONY: all remake clean cleaner resources run