: concat
 2dup 2dup ( s: ababab)
 count ( s: ababad)
 swap count dup >r + 1 + ( s: ababL r: c )
 heap-alloc dup >r ( s: ababP r: cP)
 
 rot string-copy ( s: abb r: cP )
 r> r> over >r ( s: abbPc r: P )
 + swap string-copy ( s: ab r: P )
 
 heap-free heap-free
 r>
;
