/*
 * Character mapping for HD44780 devices by Mark Haemmerling <mail@markh.de>.
 *
 * Translates ISO 8859-1 to any HD44780 charset.
 *
 * Charmap selector (C) 2006 Pillon Matteo <matteo.pillon@email.it>
 *
 * This file is released under the GNU General Public License.
 * Refer to the COPYING file distributed with this package.
 */

/*
 * HD44780 table
 *
 * Initial table taken from lcd.o Linux kernel driver by
 * Nils Faerber <nilsf@users.sourceforge.net>. Thanks!
 *
 * HD44780 charset reference: http://markh.de/hd44780-charset.png
 *
 * The following translations are being performed:
 * - map umlaut accent characters to the corresponding umlaut characters
 * - map other accent characters to the characters without accents
 * - map beta (=sharp s), micro and Yen
 *
 * Alternative mappings:
 * - #112 ("p") -> #240 (large "p"),  orig. mapped -> #112
 * - #113 ("q") -> #241 (large "q"),  orig. mapped -> #113
 *
 * HD44780 misses backslash
 *
 */

const unsigned char HD44780_charmap[] = {
        /* #0 */
          0,   1,   2,   3,   4,   5,   6,   7,
          8,   9,  10,  11,  12,  13,  14,  15,
         16,  17,  18,  19,  20,  21,  22,  23,
         24,  25,  26,  27,  28,  29,  30,  31,
        /* #32 */
         32,  33,  34,  35,  36,  37,  38,  39,
         40,  41,  42,  43,  44,  45,  46,  47,
         48,  49,  50,  51,  52,  53,  54,  55,
         56,  57,  58,  59,  60,  61,  62,  63,
        /* #64 */
         64,  65,  66,  67,  68,  69,  70,  71,
         72,  73,  74,  75,  76,  77,  78,  79,
         80,  81,  82,  83,  84,  85,  86,  87,
         88,  89,  90,  91,  47,  93,  94,  95,
        /* #96 */
         96,  97,  98,  99, 100, 101, 102, 103,
        104, 105, 106, 107, 108, 109, 110, 111,
        112, 113, 114, 115, 116, 117, 118, 119,
        120, 121, 122, 123, 124, 125, 126, 127,
        /* #128 */
        128, 129, 130, 131, 132, 133, 134, 135,
        136, 137, 138, 139, 140, 141, 142, 143,
        144, 145, 146, 147, 148, 149, 150, 151,
        152, 153, 154, 155, 156, 157, 158, 159,
        /* #160 */
        160,  33, 236, 237, 164,  92, 124, 167,
         34, 169, 170, 171, 172, 173, 174, 175,
        223, 177, 178, 179,  39, 249, 247, 165,
         44, 185, 186, 187, 188, 189, 190,  63,
        /* #192 */
         65,  65,  65,  65, 225,  65,  65,  67,
         69,  69,  69,  69,  73,  73,  73,  73,
         68,  78,  79,  79,  79,  79, 239, 120,
         48,  85,  85,  85, 245,  89, 240, 226,
        /* #224 */
         97,  97,  97,  97, 225,  97,  97,  99,
        101, 101, 101, 101, 105, 105, 105, 105,
        111, 110, 111, 111, 111, 111, 239, 253,
         48, 117, 117, 117, 245, 121, 240, 255
};

/*
 * Electronic Assembly's KS0073 based LCDs table
 * http://www.lcd-module.de/eng/pdf/doma/dip204-4e.pdf
 *
 * This map is more similar to a ISO-8859-15, but with fractions from
 * ISO-8859-1 (0xBC, 0xBD).
 *
 * ~ (126) is mapped to right arrow, even though it exists, for
 * compatibility with some clients. 127 is left arrow.
 *
 * There's no backtick, substituded with '.
 *
 * | is substituted with another similar charachter as the code is
 * used by some serial drivers.
 *
 * Charset of the display offers a nice set of icons, they are mapped
 * from 128 to 159. I mapped these intervals to, in order: 16-28,
 * 140-151, 180-182, 187, 207, 222, 224.
 *
 * (C) 2006 Pillon Matteo <matteo.pillon@email.it>
 *
 */

const unsigned char EA_KS0073_charmap[] = {
        /* #0 */
          0,   1,   2,   3,   4,   5,   6,   7,
          8,   9,  10,  11,  12,  13,  14,  15,
         16,  17,  18,  19,  20,  21,  22,  23,
         24,  25,  26,  27,  28,  29,  30,  31,
        /* #32 */
         32,  33,  34,  35,  36,  37,  38,  39,
         40,  41,  42,  43,  44,  45,  46,  47,
         48,  49,  50,  51,  52,  53,  54,  55,
         56,  57,  58,  59,  60,  61,  62,  63,
        /* #64 */
         64,  65,  66,  67,  68,  69,  70,  71,
         72,  73,  74,  75,  76,  77,  78,  79,
         80,  81,  82,  83,  84,  85,  86,  87,
         88,  89,  90, 250, 251, 252,  29, 196,
        /* #96 */
         39,  97,  98,  99, 100, 101, 102, 103,
        104, 105, 106, 107, 108, 109, 110, 111,
        112, 113, 114, 115, 116, 117, 118, 119,
        120, 121, 122, 253, 218, 255, 223, 225,
        /* #128 */
         16,  17,  18,  19,  20,  21,  22,  23,
         24,  25,  26,  27,  28, 140, 141, 142,
        143, 144, 145, 146, 147, 148, 149, 150,
        151, 180, 181, 182, 187, 207, 222, 224,
        /* #160 */
        160,  64, 177, 161,  36, 163, 243,  95,
        248,  67, 170,  20, 172, 173,  82, 175,
        128, 140, 130, 131, 249, 143, 182, 221,
        244, 129, 128,  21, 139, 138, 190,  96,
        /* #192 */
        174, 226, 174, 174,  91, 174, 188, 169,
        197, 191, 198,  69,  73,  73,  73,  73,
         68,  93, 168, 228, 236,  79,  92, 120,
        171, 238, 229, 238,  94, 230, 178, 190,
        /* #224 */
        127, 231, 175, 175, 123, 175, 189, 200,
        164, 165, 199, 101, 167, 232, 105, 105,
        111, 125, 168, 233, 237, 111, 124,  58,
        172, 166, 234, 239, 126, 235, 178, 255
};

/*
 * Character mapping by Frank Jepsen <vdr_at_jepsennet.de>.
 *
 */

const unsigned char SED1278F_0B_charmap[] = {
        /*   0 ( '^@') */    0,
        /*   1 ( '^A') */    1,
        /*   2 ( '^B') */    2,
        /*   3 ( '^C') */    3,
        /*   4 ( '^D') */    4,
        /*   5 ( '^E') */    5,
        /*   6 ( '^F') */    6,
        /*   7 ( '^G') */    7,
        /*   8 ( '^H') */    8,
        /*   9 (  '' ) */   32,
        /*  10 (  '' ) */   32,
        /*  11 ( '^K') */   32,
        /*  12 ( '^L') */   32,
        /*  13 ( '^M') */   32,
        /*  14 ( '^N') */   14,
        /*  15 ( '^O') */   15,
        /*  16 ( '^P') */   16,
        /*  17 ( '^Q') */   17,
        /*  18 ( '^R') */   18,
        /*  19 ( '^S') */   19,
        /*  20 ( '^T') */   20,
        /*  21 ( '^U') */   21,
        /*  22 ( '^V') */   22,
        /*  23 ( '^W') */   23,
        /*  24 ( '^X') */   24,
        /*  25 ( '^Y') */   25,
        /*  26 ( '^Z') */   26,
        /*  27 ( '^[') */   27,
        /*  28 ( '^\') */   28,
        /*  29 ( '^]') */   29,
        /*  30 ( '^^') */   30,
        /*  31 ( '^_') */   31,
        /*  32 ( ' ' ) */   32,
        /*  33 ( '!' ) */   33,
        /*  34 ( '"' ) */   39,
        /*  35 ( '#' ) */   35,
        /*  36 ( '$' ) */   36,
        /*  37 ( '%' ) */   37,
        /*  38 ( '&' ) */   38,
        /*  39 ( ''' ) */   39,
        /*  40 ( '(' ) */   40,
        /*  41 ( ')' ) */   41,
        /*  42 ( '*' ) */   42,
        /*  43 ( '+' ) */   43,
        /*  44 ( ',' ) */   44,
        /*  45 ( '-' ) */   45,
        /*  46 ( '.' ) */   46,
        /*  47 ( '/' ) */   47,
        /*  48 ( '0' ) */   48,
        /*  49 ( '1' ) */   49,
        /*  50 ( '2' ) */   50,
        /*  51 ( '3' ) */   51,
        /*  52 ( '4' ) */   52,
        /*  53 ( '5' ) */   53,
        /*  54 ( '6' ) */   54,
        /*  55 ( '7' ) */   55,
        /*  56 ( '8' ) */   56,
        /*  57 ( '9' ) */   57,
        /*  58 ( ':' ) */   58,
        /*  59 ( ';' ) */   59,
        /*  60 ( '<' ) */   60,
        /*  61 ( '=' ) */   61,
        /*  62 ( '>' ) */   62,
        /*  63 ( '?' ) */   63,
        /*  64 ( '@' ) */   64,
        /*  65 ( 'A' ) */   65,
        /*  66 ( 'B' ) */   66,
        /*  67 ( 'C' ) */   67,
        /*  68 ( 'D' ) */   68,
        /*  69 ( 'E' ) */   69,
        /*  70 ( 'F' ) */   70,
        /*  71 ( 'G' ) */   71,
        /*  72 ( 'H' ) */   72,
        /*  73 ( 'I' ) */   73,
        /*  74 ( 'J' ) */   74,
        /*  75 ( 'K' ) */   75,
        /*  76 ( 'L' ) */   76,
        /*  77 ( 'M' ) */   77,
        /*  78 ( 'N' ) */   78,
        /*  79 ( 'O' ) */   79,
        /*  80 ( 'P' ) */   80,
        /*  81 ( 'Q' ) */   81,
        /*  82 ( 'R' ) */   82,
        /*  83 ( 'S' ) */   83,
        /*  84 ( 'T' ) */   84,
        /*  85 ( 'U' ) */   85,
        /*  86 ( 'V' ) */   86,
        /*  87 ( 'W' ) */   87,
        /*  88 ( 'X' ) */   88,
        /*  89 ( 'Y' ) */   89,
        /*  90 ( 'Z' ) */   90,
        /*  91 ( '[' ) */   91,
        /*  92 ( '\' ) */   92,
        /*  93 ( ']' ) */   93,
        /*  94 ( '^' ) */   94,
        /*  95 ( '_' ) */   95,
        /*  96 ( '`' ) */   96,
        /*  97 ( 'a' ) */   97,
        /*  98 ( 'b' ) */   98,
        /*  99 ( 'c' ) */   99,
        /* 100 ( 'd' ) */  100,
        /* 101 ( 'e' ) */  101,
        /* 102 ( 'f' ) */  102,
        /* 103 ( 'g' ) */  103,
        /* 104 ( 'h' ) */  104,
        /* 105 ( 'i' ) */  105,
        /* 106 ( 'j' ) */  106,
        /* 107 ( 'k' ) */  107,
        /* 108 ( 'l' ) */  108,
        /* 109 ( 'm' ) */  109,
        /* 110 ( 'n' ) */  110,
        /* 111 ( 'o' ) */  111,
        /* 112 ( 'p' ) */  112,
        /* 113 ( 'q' ) */  113,
        /* 114 ( 'r' ) */  114,
        /* 115 ( 's' ) */  115,
        /* 116 ( 't' ) */  116,
        /* 117 ( 'u' ) */  117,
        /* 118 ( 'v' ) */  118,
        /* 119 ( 'w' ) */  119,
        /* 120 ( 'x' ) */  120,
        /* 121 ( 'y' ) */  121,
        /* 122 ( 'z' ) */  122,
        /* 123 ( '{' ) */  123,
        /* 124 ( '|' ) */  124,
        /* 125 ( '}' ) */  125,
        /* 126 ( '~' ) */  126,
        /* 127 ( '^?') */  127,
        /* 128 ( '~@') */  64,
        /* 129 ( '~A') */  65,
        /* 130 ( '~B') */  66,
        /* 131 ( '~C') */  67,
        /* 132 ( '~D') */  68,
        /* 133 ( '~E') */  69,
        /* 134 ( '~F') */  70,
        /* 135 ( '~G') */  71,
        /* 136 ( '~H') */  72,
        /* 137 ( '~I') */  73,
        /* 138 ( '~J') */  74,
        /* 139 ( '~K') */  75,
        /* 140 ( '~L') */  76,
        /* 141 ( '~M') */  77,
        /* 142 ( '~N') */  78,
        /* 143 ( '~O') */  79,
        /* 144 ( '~P') */  80,
        /* 145 ( '~Q') */  81,
        /* 146 ( '~R') */  82,
        /* 147 ( '~S') */  83,
        /* 148 ( '~T') */  84,
        /* 149 ( '~U') */  85,
        /* 150 ( '~V') */  86,
        /* 151 ( '~W') */  87,
        /* 152 ( '~X') */  88,
        /* 153 ( '~Y') */  89,
        /* 154 ( '~Z') */  90,
        /* 155 ( '~[') */  91,
        /* 156 ( '~\') */  92,
        /* 157 ( '~]') */  93,
        /* 158 ( '~^') */  94,
        /* 159 ( '~_') */  95,
        /* 160 ( '| ') */  124,
        /* 161 ( '�' ) */  169,
        /* 162 ( '�' ) */  164,
        /* 163 ( '�' ) */  166,
        /* 164 ( '�' ) */  175,
        /* 165 ( '�' ) */  166,
        /* 166 ( '�' ) */  124,
        /* 167 ( '�' ) */  210,
        /* 168 ( '�' ) */  177,
        /* 169 ( '�' ) */  207,
        /* 170 ( '�' ) */  178,
        /* 171 ( '�' ) */  187,
        /* 172 ( '�' ) */  44,
        /* 173 ( '�' ) */  44,
        /* 174 ( '�' ) */  206,
        /* 175 ( '�' ) */  191,
        /* 176 ( '�' ) */  178,
        /* 177 ( '�' ) */  16,
        /* 178 ( '�' ) */  30,
        /* 179 ( '�' ) */  31,
        /* 180 ( '�' ) */  180,
        /* 181 ( '�' ) */  234,
        /* 182 ( '�' ) */  211,
        /* 183 ( '�' ) */  205,
        /* 184 ( '�' ) */  44,
        /* 185 ( '�' ) */  180,
        /* 186 ( '�' ) */  178,
        /* 187 ( '�' ) */  188,
        /* 188 ( '�' ) */  182,
        /* 189 ( '�' ) */  181,
        /* 190 ( '�' ) */  245,
        /* 191 ( '�' ) */  159,
        /* 192 ( '�' ) */  65,
        /* 193 ( '�' ) */  65,
        /* 194 ( '�' ) */  65,
        /* 195 ( '�' ) */  170,
        /* 196 ( '�' ) */  142,
        /* 197 ( '�' ) */  143,
        /* 198 ( '�' ) */  146,
        /* 199 ( '�' ) */  128,
        /* 200 ( '�' ) */  69,
        /* 201 ( '�' ) */  144,
        /* 202 ( '�' ) */  69,
        /* 203 ( '�' ) */  69,
        /* 204 ( '�' ) */  73,
        /* 205 ( '�' ) */  73,
        /* 206 ( '�' ) */  73,
        /* 207 ( '�' ) */  73,
        /* 208 ( '�' ) */  245,
        /* 209 ( '�' ) */  156,
        /* 210 ( '�' ) */  79,
        /* 211 ( '�' ) */  79,
        /* 212 ( '�' ) */  79,
        /* 213 ( '�' ) */  172,
        /* 214 ( '�' ) */  153,
        /* 215 ( '�' ) */  183,
        /* 216 ( '�' ) */  174,
        /* 217 ( '�' ) */  85,
        /* 218 ( '�' ) */  85,
        /* 219 ( '�' ) */  85,
        /* 220 ( '�' ) */  154,
        /* 221 ( '�' ) */  89,
        /* 222 ( '�' ) */  220,
        /* 223 ( '�' ) */  224,
        /* 224 ( '�' ) */  133,
        /* 225 ( '�' ) */  160,
        /* 226 ( '�' ) */  131,
        /* 227 ( '�' ) */  171,
        /* 228 ( '�' ) */  132,
        /* 229 ( '�' ) */  134,
        /* 230 ( '�' ) */  145,
        /* 231 ( '�' ) */  135,
        /* 232 ( '�' ) */  138,
        /* 233 ( '�' ) */  130,
        /* 234 ( '�' ) */  136,
        /* 235 ( '�' ) */  137,
        /* 236 ( '�' ) */  141,
        /* 237 ( '�' ) */  161,
        /* 238 ( '�' ) */  140,
        /* 239 ( '�' ) */  139,
        /* 240 ( '�' ) */  32,
        /* 241 ( '�' ) */  155,
        /* 242 ( '�' ) */  149,
        /* 243 ( '�' ) */  162,
        /* 244 ( '�' ) */  147,
        /* 245 ( '�' ) */  173,
        /* 246 ( '�' ) */  148,
        /* 247 ( '�' ) */  184,
        /* 248 ( '�' ) */  175,
        /* 249 ( '�' ) */  151,
        /* 250 ( '�' ) */  163,
        /* 251 ( '�' ) */  150,
        /* 252 ( '�' ) */  129,
        /* 253 ( '�' ) */  121,
        /* 254 ( '�' ) */  32,
        /* 255 ( '�' ) */  253
};

#define MAX_CHARMAP_NAME_LENGHT 16

struct charmap {
        char name[MAX_CHARMAP_NAME_LENGHT];
        const unsigned char *charmap;
};

const struct charmap available_charmaps[] = {
	{ "hd44780_default", HD44780_charmap     },
	{ "ea_ks0073",       EA_KS0073_charmap   },
	{ "sed1278f_0b",     SED1278F_0B_charmap }
};