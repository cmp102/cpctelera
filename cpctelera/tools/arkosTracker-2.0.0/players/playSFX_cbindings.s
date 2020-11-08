
_cpct_akp2PlaySFX::
	ld  hl, #2                                   ;; [10] Get Parameter from Stack
	add hl, sp                                   ;; [11]
    ld  a, (hl)                                  ;; [ 7] A = Sound effect number
	inc hl                                       ;; [ 6]
    ld  c, (hl)                                  ;; [ 7] C = channel
	inc hl                                       ;; [ 6]
	ld  b, (hl)                                  ;; [ 7] B = inverted volume
    
