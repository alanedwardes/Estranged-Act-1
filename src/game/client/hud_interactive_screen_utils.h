#include "cbase.h"
 
// ae - takes URL, puts percents in it.
void encodeUrl(char *out, int outSize, char *in, int inSize)
{
	// Terminate our buffer
	out[0] = '\0';

	// Loop the input
	for (int i = 0; i < inSize; i++)
	{
		// Get a char out of it
		unsigned char c = in[i];

		// Characters to exclude from this conversion
		// These are for the requirements of this project,
		// there is an RFC somewhere with what's meant to be here.
		if ((c >= 46 && c <= 57)  || // . / 0-9
			(c >= 65 && c <= 90)  || // A-Z
			(c >= 97 && c <= 122) || // a-z
			(c == 92) || // .
			(c == 58)) // :
		{
			// Just add the char as-is
			Q_snprintf(out, outSize, "%s%c", out, c);
		}
		else
		{
			// Prepend a percent symbol, hex it
			Q_snprintf(out, outSize, "%s%c%02X", out, '%', c);
		}
	}
}