.ORIG x3000
MAIN    LD R6, STACK   ; Set stack address
        LDI R3, RA
        NOT R3, R3
        ADD R3, R3, #1
        ST R3, R       ; Store -R in 'R'
        LDI R4, CA
        ST R4, NC      ; Store C in 'NC'
        NOT R4, R4
        ADD R4, R4, #1
        ST R4, C       ; Store -C in 'C'
        AND R1, R1, #0
        LD R0, TTMAX   ; Load offset
        LD R5, AREA
        ADD R5, R5, R0 ; Calculate address (p)
        ADD R1, R1, #0
FORI    ADD R0, R1, R3 ; for i from 0 to R
        BRzp EFORI     ; break if i == R
        AND R2, R2, #0
FORJ    ADD R0, R2, R4 ; for j from 0 to C
        BRzp EFORJ     ; break if j == C
        AND R0, R0, #0
        STR R0, R5, #0 ; Initializ result array
        ADD R2, R2, #1 ; j++
        ADD R5, R5, #1 ; p++
        BRnzp FORJ
EFORJ   ADD R1, R1, #1 ; i++
        BRnzp FORI
EFORI   LD R5, C
        LD R3, AREA    ; load address (p)
        ; Store max in R4, which is nagative noew
        AND R1, R1, #0 ; for i
FORI2   LD R0, R
        ADD R0, R1, R0
        BRzp EFORI2
        AND R2, R2, #0
FORJ2   ADD R0, R2, R5 ; for j
        BRzp EFORJ2
        JSR DPFUN      ; call dp(i, j, p)
        ADD R3, R3, #1 ; p++
        NOT R7, R0
        ADD R7, R7, #1
        ADD R7, R4, R7 ; If new result bigger than max?
        BRzp MSKIP     ; No, keip
        ADD R4, R0, #0 ; Otherwise update max
MSKIP   ADD R2, R2, #1 ; j++
        BRnzp FORJ2
EFORJ2  ADD R1, R1, #1 ; i++
        BRnzp FORI2
EFORI2  ADD R2, R4, #0 ; Store result in R2
        TRAP x25       ; Halt
TTMAX   .FILL #10000
RA      .FILL x3200
CA      .FILL x3201
R       .BLKW 1
C       .BLKW 1
NC      .BLKW 1
AREA    .FILL x3202
STACK   .FILL xFE00    

DPFUN   ADD R6, R6, #-6
        STR R1, R6, #5
        STR R2, R6, #4
        STR R3, R6, #3
        STR R4, R6, #2
        STR R5, R6, #1
        STR R7, R6, #0 ; Sotre register in stack
        LD R0, TTMAX
        ADD R0, R3, R0
        LDR R4, R0, #0 ; If already calculated, return
        BRp DPRET
        LDR R5, R3, #0
        LD R0, C
        ADD R3, R3, R0
        ADD R1, R1, #-1 ; now we have i-1, p-C
        BRn DPNEXT1     ; If out of border, skip
        JSR DPCMP       ; check height
        BRnz DPNEXT1	; skip
        JSR DPFUN       ; otherwise call dp(i-1, j, p-C)
        JSR SUBDP       ; update max
DPNEXT1 LD R0, NC
        ADD R3, R3, R0
        ADD R3, R3, R0
        ADD R1, R1, #2  ; now we have i+1, p+C
        LD R0, R
        ADD R0, R1, R0
        BRzp DPNEXT2    ; Check border
        JSR DPCMP       ; check height
        BRnz DPNEXT2
        JSR DPFUN       ; call dp(i+1, j, p+C)
        JSR SUBDP       ; update max
DPNEXT2 LD R0, C
        ADD R3, R3, R0
        ADD R1, R1, #-1
        ADD R3, R3, #-1
        ADD R2, R2, #-1 ; now we have j-1, P-1
        BRn DPNEXT3     ; Check border
        JSR DPCMP       ; check height
        BRnz DPNEXT3	
        JSR DPFUN       ; call dp(i, j-1, p-1)
        JSR SUBDP       ; update max
DPNEXT3 ADD R3, R3, #2
        ADD R2, R2, #2  ; now we have j+1, P+1
        LD R0, C
        ADD R0, R2, R0
        BRzp EDPFUN     ; Check border
        JSR DPCMP       ; check height
        BRnz EDPFUN
        JSR DPFUN       ; call dp(i, j+1, p+1)
        JSR SUBDP       ; update max
EDPFUN  ADD R3, R3, #-1
        LD R0, TTMAX
        ADD R3, R3, R0
        ADD R4, R4, #1
        STR R4, R3, #0  ; Store the result
DPRET   ADD R0, R4, #0  ; move result to R0
        LDR R1, R6, #5
        LDR R2, R6, #4
        LDR R3, R6, #3
        LDR R4, R6, #2
        LDR R5, R6, #1
        LDR R7, R6, #0
        ADD R6, R6, #6
        ADD R0, R0, #0
        RET             ; Restore everything and return
; Check the height
DPCMP   LDR R0, R3, #0  ; load new height
        NOT R0, R0;
        ADD R0, R0, #1
        ADD R0, R0, R5  ; compare
        RET
SUBDP   ST R5, DPST5 
        NOT R5, R0;
        ADD R5, R5, #1
        ADD R5, R4, R5  ; compare max and the result
        BRzp SUBRET
        ADD R4, R0, #0  ; update max
SUBRET  LD R5, DPST5    ; return
        RET
DPST5   .BLKW 1