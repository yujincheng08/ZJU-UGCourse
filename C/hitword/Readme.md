1. 屏幕上同时显示的单词个数必须≤24个, 每行最多显示一个单词。
   因为文本模式下整屏一共是25行, 底行用来显示成绩, 上面只剩
   24行。

2. 每个单词首字母必须各不相同, 否则用户在敲第一个字母时, 
   程序将无法选择该把哪个单词的首字母涂成绿色。

3. 各单词可以在固定的25个中随机选取，也可以用函数read_dic()
   从字典dict.idx中随机读取:

```C
typedef struct 
{
   char entry[20];  /* 条目, 单词最长20个字母; 当单词刚好20个字母时, 不用'\0'结束 */
   long lib_offset; /* 单词解释的偏移量(该偏移量是指字典正文文件dict.lib中的fseek距离) */
   short int  xlat_len;   /* 单词解释占用的字节数 */
   short int  exam_len;   /* 单词例句占用的字节数 */
} IDX; 

IDX *pdic;        // 字典中首个单词指针
int entries;      // 字典中的单词个数
WORD w[25];       // 显示在屏幕上的单词
int index[26][2]; // index[i][0]: ('a'+i)字母开头的首个单词在字典中的序号
                  // index[i][1]: ('a'+i)字母开头的单词个数               
int screen_words; // 屏幕上已显示的单词总数
int hit=-1, hit_len=0; // hit=当前打中的单词下标, hit_len=已打中的字母个数
int stop = 0;
int words_list=0, words_hit=0, words_lost=0;
float hit_rate=0;

int read_dic(IDX **ppdic)
{
   FILE *fp;
   IDX *pdic;
   int len, entries;
   int i, begin;
   fp = fopen("dict.idx", "rb");
   if(fp == NULL)
      return 0;
   fseek(fp, 0, SEEK_END);
   len = ftell(fp);
   fseek(fp, 0, SEEK_SET);
   entries = len / sizeof(IDX);
   pdic = malloc(entries*sizeof(IDX));
   if(pdic == NULL)
      return 0;
   fread(pdic, sizeof(IDX), entries, fp);
   fclose(fp);
   *ppdic = pdic;
   begin = 0;
   for(i=0; i<26; i++)
   {
      while(begin < entries)
      {
         if((pdic[begin].entry[0] | 0x20) == 'a'+i)
            break;
         begin++;
      }
      index[i][0] = begin;
      if(i>0)
         index[i-1][1] = begin - index[i-1][0];
   }
   index[i-1][1] = entries - begin;
   return entries;
}

int all_alpha(char *p)
{
   int i, n=strlen(p);
   for(i=0; i<n; i++)
   {
      if(!islower(p[i]))
         break;
   }
   return i==n;
}

void generate_one_word(void)
{
   int i, j, d;
   int mark[26]={0};
   for(j=0; j<screen_words; j++)
   {
      mark[(w[j].word[0] | 0x20) - 'a'] = 1;
   }
   d = random(100) & 1;
   if(d==0)
   {
      for(j=0; j<26; j++)
         if(mark[j] == 0)
            break;
   }
   else
   {
      for(j=25; j>=0; j--)
         if(mark[j] == 0)
            break;
   }
   do
   {
      i = random(index[j][1])+index[j][0];
   } while(!all_alpha(pdic[i].entry));

   memset(w[screen_words].word, 0, sizeof(w[0].word));
   strncpy(w[screen_words].word, pdic[i].entry, 20);
   w[screen_words].y = 0;
   w[screen_words].x = random(80-strlen(w[screen_words].word));
   screen_words++;
   words_list++;
}

main()
{
   int key, i;
   randomize(); // 随机数初始化
   entries = read_dic(&pdic); // 读取字典的条目
                              // entries=单词个数
                              // pdic->首个单词
   memset(w, 0, sizeof(w));   // 初始化屏幕上的25个单词
   screen_words = 0;          // 屏幕上已显示的单词个数
   clrscr(); // 清屏

   while(!stop)               // 当按Esc时, stop=1
   {
      generate_one_word();    // 数组末尾增加一个新单词
      move_words_down();      // 屏幕上各单词往下移动一行
      show_score();           // 显示成绩
      delay(500);             // 延时等待500毫秒
      while(bioskey(1) != 0)  // if the keyboard buffer holds some keys
      {
         key = bioskey(0) & 0xFF; // convert scan code to ASCII
         if(key == 0x1B) // Esc is pressed
         {
            stop = 1;
            break;
         }
    	   // 此处省略若干代码...
      } // while(bioskey(1) != 0)
   } // while(!stop)
   clrscr();
   puts("Done!");
   return 0;
}
```

