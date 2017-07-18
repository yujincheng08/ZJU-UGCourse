.ORIG x3000

; Main program
MAIN    LEA R0, PROMPT  ; Load PROMPT address
        TRAP x22        ; Output PROMPT string
        LEA R1, IADDR   ; Load the address of input string array
        JSR INPUT       ; Call INPUT function
        LD R2, TADDR    ; Load the first node
        JSR TRAV        ; Call TRAV function
        BRnp MAINEND    ; Output result != 0, no output "No Entry\n"
        LEA R0, NOENTRY ; Nothing output, output "No Entry\n"
        TRAP x22        ; Trap for outputting
MAINEND TRAP x25        ; Halt
PROMPT  .STRINGZ "Type a name and press Enter: "
NOENTRY .STRINGZ "No Entry\n"
TADDR   .FILL 0x4000    ; Address of the first node of the linked list
IADDR   .BLKW 16        ; Array for input string


; Travel linked list, compare string stroes in R1, list address in R2, return output count in R0
TRAV    ST R7, TRAVST7
        ST R2, TRAVST2
        ST R3, TRAVST3  ; Store register before using them
        AND R3, R3, #0  ; Log the number of nodes output
TRVLOOP LDR R2, R2, #0  ; Load the next node pointer
        BRz TRAVEND     ; Touch the end of the list, break the loop
        JSR CMPNODE     ; Not the end, then check the node
        BRnp TRVLOOP    ; Not matched, then continue for loop
        ST R1, TRAVST1  ; Matched, call OPTNODE function
        ADD R1, R2, #0
        JSR OPTNODE
        ADD R3, R3, #1  ; Increment the counter
        LD R1, TRAVST1  ; Restore R1 after calling the function
        BRnzp TRVLOOP   ; Continue for the next loop
TRAVEND LD R7, TRAVST7
        LD R2, TRAVST2
        ADD R0, R3, #0
        LD R3, TRAVST3  ; Restore all registers
        ADD R0, R0, #0  ; Maintain R0 is the last to stored
        RET
TRAVST1 .BLKW 1
TRAVST2 .BLKW 1
TRAVST3 .BLKW 1
TRAVST7 .BLKW 1         ; Space to store registers


; Compare node whose address stores in R2. Compare string stores in R1
CMPNODE ST R2, NCMP2
        ST R7, NCMP7
        ST R3, NCMP3    ; Store all registers before using them
        ADD R3, R2, #0  ; Using R3 instead of R2
        LDR R2, R3, #2  ; Load first name
        JSR STRCMP      ; Call STRCMP
        BRz ENDNCMP     ; It matched, break the loop
        LDR R2, R3, #3  ; Not matched, try to compare last name
        JSR STRCMP      ; Call STRCMP
ENDNCMP LD R2, NCMP2
        LD R7, NCMP7
        LD R3, NCMP3    ; Restore all registers
        ADD R0, R0, #0  ; Maintain R0 is the last registers to store
        RET
NCMP2   .BLKW 1
NCMP3   .BLKW 1
NCMP7   .BLKW 1         ; Space to store registers


; Output node. Address stores in R1
OPTNODE ST R1, OPNST1
        ST R7, OPNST7   ; Store registers before using them
        ADD R1, R1, #3
        LDR R0, R1, #0  ; Load last name
        TRAP x22        ; Output
        LD R0, SPACE
        TRAP x21        ; Output <space>
        ADD R1, R1, #-1
        LDR R0, R1, #0  ; Load first name
        TRAP x22        ; Output
        LD R0, SPACE    ; Output <space>
        TRAP x21
        ADD R1, R1, #-1 ; Load room
        LDR R0, R1, #0
        TRAP x22        ; Output
        LD R0, ENTER    ; Output <enter>
        TRAP x21
        LD R1, OPNST1
        LD R7, OPNST7   ; Restore all registers
        RET
OPNST1  .BLKW 1
OPNST7  .BLKW 1         ; Space to store registers
SPACE   .FILL x0020     ; ASCII for <space>
ENTER   .FILL x000A     ; ASCII for <enter>


; Input string to R1
INPUT   ST R7, INPTST7
        ST R1, INPTST1  ; Store registers
	ST R2, INPTST2	;
	LD R2, SUPPORT	;
ILOOP   TRAP x20        ; Read one character
	;AND R0, R0, R2
        TRAP x21        ; Print it onto the screen
        ADD R2, R0, #-10; Check if it's <enter>
        BRz IEND        ; If so, break the loop
        STR R0, R1, #0  ; Otherwise, store it to the memory
        ADD R1, R1, #1  ; Move the pointer
        BRnzp ILOOP     ; Continue for the loop
IEND    AND R0, R0, #0
        STR R0, R1, #0  ; Store null to the last
        LD R1, INPTST1
	LD R2, INPTST2
        LD R7, INPTST7  ; Load all the registers
        RET
SUPPORT	.FILL x00FF
INPTST7 .BLKW 1
INPTST1 .BLKW 1
INPTST2 .BLKW 1         ; Space to store registers

; Compare two strings store in R1 and R2, result stored in R0
STRCMP  ST R1, CMPST1
        ST R2, CMPST2
        ST R3, CMPST3
        ST R4, CMPST4   ; Store registers
        AND R0, R0, #0  ; Initialize the result
CMPLOOP LDR R3, R1, #0
        LDR R4, R2, #0  ; Load two characters
        NOT R4, R4
        ADD R4, R4, #1
        ADD R0, R3, R4  ; Substracte them and store the result to R0
        BRnp CMPEND     ; If they are not equal,
        ADD R3, R3, #0
        BRz CMPEND      ; or R3 is null,
        ADD R4, R4, #0
        BRz CMPEND      ; or R4 is null, break the loop
        ADD R1, R1, #1
        ADD R2, R2, #1  ; Otherwise move pointers
        BRnzp CMPLOOP   ; And continue for the next loop
CMPEND  LD R1, CMPST1
        LD R2, CMPST2
        LD R3, CMPST3
        LD R4, CMPST4   ; Restore the registers
        ADD R0, R0, #0  ; Maintain R0 is the last register to store
        RET
CMPST1 .BLKW 1
CMPST2 .BLKW 1
CMPST3 .BLKW 1
CMPST4 .BLKW 1          ; Space to store registers

.END