#/bin/bash
glslangValidator test1.comp -DMULTIPLIER=2 -V -I. -e main -g -o test1.spv
glslangValidator test2.vert -DMULTIPLIER=2 -V -I. -e main -g -o test2_vert.spv
glslangValidator test2.frag -DMULTIPLIER=2 -V -I. -e main -g -o test2_frag.spv
# -g0 strip debug information

# -vn is for debugging only and creates a c header file with the data
glslangValidator test1.comp -DMULTIPLIER=2 -V -I. -e main -g -o test1_spv_data.h --vn test1_spv_data
glslangValidator test2.vert -DMULTIPLIER=2 -V -I. -e main -g -o test2_vert_spv_data.h --vn test2_vert_spv_data
glslangValidator test2.frag -DMULTIPLIER=2 -V -I. -e main -g -o test2_frag_spv_data.h --vn test2_frag_spv_data
