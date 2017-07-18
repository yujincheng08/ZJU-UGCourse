.ORIG x2000
KBI     ST R0, KBIST0
        ST R1, KBIST1
        ST R2, KBIST2
        ST R7, KBIST7    ; Save registers before starting
        AND R2, R2, #0
        ADD R2, R2, #10  ; Initial R2 to 10
        LDI R0, KBDR     ; Get ASCII from KBDR
KBILOOP	JSR OUTPUT       ; Output the character onece
        ADD R2, R2, #-1  ; Decrease counter
        BRp KBILOOP      ; Loop for 10 time.
        LD R0, SPACE
        JSR OUTPUT       ; Output <enter>
        LD R7, KBIST7    ; Restore registers
        LD R2, KBIST2
        LD R0, KBIST0
        LD R1, KBIST1
        NOT R5, R5       ; Trigger status
        RTI              ; Return from interrupt
KBIST0  .BLKW 1
KBIST1  .BLKW 1
KBIST2  .BLKW 1
KBIST7  .BLKW 1
SPACE   .FILL x000A
KBDR    .FILL xFE02

; For interrupt output
OUTPUT  ST R1, OPTST1
OLOOP   LDI R1, DSR
        BRzp OLOOP
        STI R0, DDR
        LD R1, OPTST1
        RET
OPTST1  .BLKW 1
DSR     .FILL xFE04
DDR	    .FILL xFE06
.END
