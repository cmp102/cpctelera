
_cpct_akp2Init::
	ld  hl, #2                                   ;; [10] Get Parameter from Stack
	add hl, sp                                   ;; [11]
	ld  a, (hl)                                  ;; [ 7]
	inc hl                                       ;; [ 6]
    ld  h, (hl)                                  ;; [ 7]
	ld l, a                                      ;; [ 4] HL = pointer to the song
	xor a                                        ;; [ 4] Selects subsong 0
 
