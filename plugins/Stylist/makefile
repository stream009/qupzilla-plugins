include Makefile

pch:
	grep -hr "#include <" --include="*.cpp" --include="*.h" --exclude-dir=test | sort | uniq > stable.h
