#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t magic;  // 0x46546C67
    uint32_t version;
    uint32_t length;
} GLBHeader;

typedef struct {
    uint32_t chunkLength;
    uint32_t chunkType;
} GLBChunkHeader;

void printWord(char* data, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        printf("%c", data[i]);
    }
    printf("\n");
}

// Parse the GLB JSON chunk.
// How shall we store the data? A Hasmap maybe?
// Or if we know what sutructure the data is packed in it, we might just use
// that as it is, konwing everything beforehand. That would be great!
void parseJSON(char* json, uint32_t length)
{
    int totalWords = 0;
    for (size_t i = 0; i < length; i++)
    {
        int startIndex = i;
        int endIndex = i;
        if (json[i] == '{') 
        {
            startIndex = ++i;
            while (json[i] != '}')
            {
                i += 1;
            }

            endIndex = i;
            totalWords += 1;
            printWord(json, startIndex, endIndex);

        }
    }
    printf("Total Words found in double qoutations: %d\n", totalWords);
}

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

    parseJSON(jsonData, jsonChunk.chunkLength);

    printf("INFO: JSON length: %d\n", jsonChunk.chunkLength);
    printf("INFO: %s\n", jsonData);

    fclose(file);

    return 0;
}
