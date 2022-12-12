ENTRY(_start)

MEMORY {
	BIOS_RAM    (rwx) : ORIGIN = 0x80000000, LENGTH = 0x010000
	RAM         (rwx) : ORIGIN = 0x80010000, LENGTH = 0x7f0000 /*8MB RAM*/ /*LENGTH = 0x1f0000  for 2MB RAM*/
    SCRATCH_RAM (rwx) : ORIGIN = 0x1F800000, LENGTH = 0x000400
}

SECTIONS {
    /* The program code and other data goes into RAM */
	.text : {
		__text_start = .;
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */

        KEEP (*(.init))
        KEEP (*(.fini))
        . = ALIGN(4);
        _etext = .;
	} > RAM

    /* Constant data */
	.rodata : {
        . = ALIGN(4);
		*(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */

        . = ALIGN(16);
        __CTOR_LIST__ = .;
		LONG(((__CTOR_END__ - __CTOR_LIST__) / 4) - 2)
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		LONG(0)
		__CTOR_END__ = .;

        . = ALIGN(16);
        __DTOR_LIST__ = .;
		LONG(((__DTOR_END__ - __DTOR_LIST__) / 4) - 2)
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		LONG(0)
		__DTOR_END__ = .;

        . = ALIGN(4);
	} > RAM


    /*Omg, init/fini arrays not supported for mips target o_O
    Enable somehow HAVE_INITFINI_ARRAY_SUPPORT  while compiling GCC 
    --enable-initfini-array ???*/
    /*.preinit_array     :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >RAM
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >RAM
    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >RAM*/

    _sidata = LOADADDR(.data);
	.data : {
        . = ALIGN(4);
        _sdata = .;
		*(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        . = ALIGN(4);
        edata = .;
	} > RAM


	HIDDEN(_gp = ALIGN(16) + 0x7ff0);

	.sdata : {
		*(.sdata .sdata.*)
	} > RAM

	. = ALIGN(4);
	__bss_start = .;

	.sbss (NOLOAD) : {
        _sbss_start = .;
		*(.sbss)
        *(.sbss.*)
		*(.scommon)
        . = ALIGN(4);
        _sbss_end = .;
	} > RAM
	.bss (NOLOAD) : {
        _bss_start = .;
		*(.bss .bss.*)
		*(COMMON)
        . = ALIGN(4);
        _bss_end = .;
	} > RAM

	. = ALIGN(4);
	_end = .;
    __bss_end = .;

	.dummy (NOLOAD) : {
		KEEP(*(.dummy))
	} > RAM

	/DISCARD/ : {
		*(.gnu.lto_*)
	}
}
