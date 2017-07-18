.ORIG x3000
SP      LEA R6, SP      ; Initialize the stack pointer
        LD R1, KBIA
        STI R1, KBIV    ; Set up the keyboard interrupt vector table entry
        LD R1, KBSRSET
        STI R1, KBSR    ; Enable keyboard interrupts
        LEA R1, SSCHKB
        LEA R2, CHKBADR
        STR R1, R2, #-1
        LEA R1, XXCHKB
        STR R1, R2, #0  ; Prepare address of the status denotes
        AND R5, R5, #0	; Initialize R5, which is the status
LOOP    LEA R2, CHKBADR ; Base address of status table
        ADD R2, R2, R5
        LDR R0, R2, #0  ; Load pattern accroding to status
        TRAP x22        ; Output
        JSR DELAY
        BRnzp LOOP      ; Infinte loop after delay
        .BLKW 1
CHKBADR .BLKW 1
KBIV    .FILL x0180	;
KBIA    .FILL x2000	;
KBSR    .FILL xFE00	;
KBSRSET .FILL x4000	;
XXCHKB  .STRINGZ "**    **    **    **    **    **    **    **\n   **    **    **    **    **    **    **    \n"
SSCHKB  .STRINGZ "##    ##    ##    ##    ##    ##    ##    ##\n   ##    ##    ##    ##    ##    ##    ##    \n"

; Delay fuction, pre-provided
DELAY   ST R1, DLYST
        LD R1, DLYCNT
DLYREP  ADD R1, R1, #-1
        BRp DLYREP
        LD R1, DLYST
        RET
DLYCNT  .FILL #2500
DLYST   .BLKW 1

.END
