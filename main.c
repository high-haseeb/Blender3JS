#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint32_t magic;  // 0x46546C67
    uint32_t version;
    uint32_t length;
} GLBHeader;

typedef struct {
    uint32_t chunkLength;
    uint32_t chunkType;
} GLBChunkHeader;

int main(void) 
{
    const char* filepath = "./assets/Cube.glb";
    FILE *file = fopen(filepath, "rb");

    if (!file) {
        printf("ERORR: Can not open file %s", filepath);
        return -1;
    }

    GLBHeader header;
    fread(&header, sizeof(GLBHeader), 1, file);

    if (header.magic != 0x46546C67) {
        printf("ERROR: Not a GLB file\n");
        fclose(file);
        return -1;
    }

    printf("INFO: GLB version %d\n", header.version);
    printf("INFO: GLB length %d\n", header.length);

    // After the GLB header we have two GLB chunks.
    // First is the JSON chunk and the other is Binary chunk.
    // The JSON chunk defines how the data in the Binary chunk is used to 
    // create the scene. We can load the Binary chunk direcly in javascript
    // and based on the JSON chunk generate the code.
    GLBChunkHeader jsonChunk;
    fread(&jsonChunk, sizeof(jsonChunk), 1, file);
    char *jsonData = malloc(jsonChunk.chunkLength + 1);
    fread(jsonData, 1, jsonChunk.chunkLength, file);
    jsonData[jsonChunk.chunkLength] = '\0'; 

    printf("INFO: JSON length: %d\n", jsonChunk.chunkLength);
    printf("INFO: JSON data:\n%s\n", jsonData);

    fclose(file);

    return 0;
}
