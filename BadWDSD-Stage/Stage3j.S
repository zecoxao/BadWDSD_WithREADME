.global stage_entry

// 0x2401F031200

stage_entry:
    li %r11, 0x120
    ld %r11, 0(%r11)
    mtctr %r11
    bctr