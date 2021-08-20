FILE=$(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_path:= $(abspath $(lastword $(MAKEFILE_LIST)))
BASE:=$(dir $(mkfile_path))

all:
	cd $(BASE)memory && make
	cd $(BASE)ms && make

clean:
	find . -name *.o |xargs rm
	find . -name main|xargs rm
