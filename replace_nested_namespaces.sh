#!/bin/bash
#clang-format-5.0 -style="{FixNamespaceComments: true, IndentWidth: 4, PointerAlignment: Left}" -i $1
clang-format-5.0 -style=file -i $1
perl -0 -p -e "s/namespace ([a-z]*)::([a-z]*)::([a-z]*) \{\n([^\n]*)\n}/namespace \1 { namespace \2 { namespace \3 {\n\4\n}}}/g" -i $1
perl -0 -p -e "s/namespace ([a-z]*)::([a-z]*) \{\n([^\n]*)\n}/namespace \1 { namespace \2 {\n\3\n}}/g" -i $1
perl -0 -p -e "s/namespace ([a-z]*)::([a-z]*)::([a-z]*) \{/namespace \1 { namespace \2 { namespace \3 {/g" -i $1
perl -0 -p -e "s|} // namespace [a-z]*::[a-z]*::[a-z]*|}}} // namespace|g" -i $1
perl -0 -p -e "s/namespace ([a-z]*)::([a-z]*) \{/namespace \1 { namespace \2 {/g" -i $1
perl -0 -p -e "s|} // namespace [a-z]*::[a-z]*|}} // namespace|g" -i $1
