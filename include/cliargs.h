extern unsigned char verbose_flag;
extern unsigned char help_flag;
extern unsigned char file_flag;
extern unsigned char version_flag;

extern unsigned char MAX_DATA_LENGTH;
extern unsigned short INTIAL_ADDRESS;
extern char *VERSION ;
extern char *FILENAME;

int handleCLIargs(int argc, char *argv[]);
int versionHandler(void);
int helpHandler(void);

