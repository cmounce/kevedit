#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "zzt.h"


/**** libzzt2 block printing */

void printblock(ZZTblock * b) {
	int x, y;
	for (y = 0; y < b->height; y++) {
		for (x = 0; x < b->width; x++) {
			char c = zztTileGetDisplayChar(b, x, y);
			if (c != '\n')
				printf("%c", c);
			else
				printf(" ");
		}
		printf("\n");
	}
}

void printboard(ZZTworld * w) {
	printblock(zztBoardGetCurPtr(w)->bigboard);
}

void shuffle(ZZTworld * w) {
	int i;
	float bcount;
	int src, dest;

	/* Initialize random number generator */
	srand(time(NULL));
	/* Swap random boards for a while */
	bcount = zztWorldGetBoardcount(w) - 1.0;
	for(i = 0; i < 100; i++) {
		src = (int) 1 + (bcount*rand()/(RAND_MAX+1.0));
		dest = (int) 1 + (bcount*rand()/(RAND_MAX+1.0));
		printf("%i -> %i\n", src, dest);
		zztWorldMoveBoard(w, src, dest);
	}
}

/****** General libzzt2 tests */

void testblockcopy(ZZTworld * w) {
	ZZTblock *area;

	/* block copy testing */
	area = zztBlockCopyArea(zztBoardGetBlock(w), 17, 10, 57, 15);
	printblock(area);
	zztBlockPaste(zztBoardGetBlock(w), area, 0, 8);

	zztBlockFree(area);
}

void testparams(ZZTworld * w) {
	zztBoardSelect(w, 1);
	zztMove(w, 0, 0, 0, 1);
	zztPlotPlayer(w, 1, 0);
}

void testworld(char * filename)
{
	ZZTworld *w;
	char loadname[1000] = "town.zzt";

	if (filename != NULL)
		strcpy(loadname, filename);

	/* Load world */
	w = zztWorldLoad(loadname);
	if(w == NULL) {
		printf("Error loading world!\n");
		exit(1);
	}
	printf("Loaded %s\n", loadname);

	/* Test something */
	testparams(w);

	/* Print the current board */
	printboard(w);

	/* Rename to testout */
	zztWorldSetFilename(w, "testout.zzt");
	zztWorldSetTitle(w, "testout");

	/* Unlock */
	zztWorldSetFlag(w, 0, "");

	/* Save */
	if(!zztWorldSave(w))
		printf("Error saving world!\n");
	else
		printf("Saved test\n");

	/* Clean up */
	zztWorldFree(w);
}

/***** zztoop parser tests */
#include "zztoop.h"

void testTokenizer(char * line)
{
	ZZTOOPparser * parser = zztoopCreateParser(line);

	printf("Tokenizing \"%s\":\n", parser->line);
	while (zztoopNextToken(parser)) {
		printf("\"%s\"\n", parser->token);
	}

	zztoopDeleteParser(parser);
}

void printComponents(ZZTOOPcomponent* comp)
{
	char * text = "NULL";

	if (comp == NULL)
		return;

	if (comp->text != NULL)
		text = comp->text;

	printf("Pos: %d, Type: %s, value: %d, text: \"%s\"\n", comp->pos, zztoopTypeDescription(comp->type), comp->value, text);

	printComponents(comp->next);
}

void testParser(char * line)
{
	ZZTOOPparser * parser = zztoopCreateParser(line);

	parser->flags = ZOOPFLAG_HELP;

	printf("Parsing (help) \"%s\":\n", line);
	printComponents(zztoopParseLine(parser));

	zztoopDeleteParser(parser);
}

void testParserMany(void)
{
	testParser("#go north");
	testParser("/i/i#change key door");
	testParser("!message;text 'yeah");
	testParser(":label;text");
	testParser("!object:message;text");
	testParser("!-file:message;text");
	testParser(":label   'Comment");
	testParser(":label #go north");
}

int main(int argc, char * argv[])
{
	if (argc > 1)
		testParser(argv[1]);
	else
		testParserMany();

	return 0;
}

