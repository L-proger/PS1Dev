.section .data

.global Texture_Test_64x64
.type Texture_Test_64x64, @object
.align 4
Texture_Test_64x64:
    .incbin "textures/Test_64x64.tim"

.global Texture_Test_64x64_Size
.type Texture_Test_64x64_Size, @object
.align 4
Texture_Test_64x64_Size:
    .int Texture_Test_64x64_Size - Texture_Test_64x64
