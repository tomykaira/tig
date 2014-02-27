#!/bin/sh

sed -n '/^\[\[/,/\(---\|~~~\)/p' < "$1" | while read line; do \
	case "$$line" in \
	"----"*)  echo ". <<$$ref>>"; ref= ;; \
	"~~~~"*)  echo "- <<$$ref>>"; ref= ;; \
	"[["*"]]") ref="$$line" ;; \
	*)	   ref="$$ref, $$line" ;; \
	esac; done | sed 's/\[\[\(.*\)\]\]/\1/'
