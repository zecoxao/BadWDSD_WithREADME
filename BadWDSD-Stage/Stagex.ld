SECTIONS
{
    . = 0x2401F031000;
    entry1 : { *(entry1) }

    . = 0x2401F031100;
    entry2 : { *(entry2) }

    . = 0x2401F031200;
    entry3 : { *(entry3) }

    . = 0x2401F031400;
    entry5 : { *(entry5) }

    . = 0x2401F031600;
    .toc : { *(.toc) }

    . = 0x2401F031610;
    bcode : { *(bcode) }

    main1 : { *(main1) }
    main2 : { *(main2) }
    main3 : { *(main3) }
    main5 : { *(main5) }

    code : { *(code) }

    .text : { *(.text) }

    .opd : { *(.opd) }
}