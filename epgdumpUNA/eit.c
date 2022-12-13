#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "util.h"
#include "eit.h"
#include "ts_ctl.h"

#ifndef KATAUNA
char		*subtitle_cnv_str[] = {
	NULL
};
#endif


extern EIT_CONTROL	*free_EIT_CONTROL( EIT_CONTROL * );


#if MoveAribMark
#define ALPHA_CHK(A,B) ( ( isupper(*(A)) && isupper(*(B)) ) || ( islower(*(A)) && islower(*(B)) ) )

char	stock[120];
int		treat_mode = OFF;
int		stock_mode;


int ev_flg_move( char *src, char *target, char *add_char )
{
	int	src_lp = strlen( src );
	int	tgt_ln = strlen( target );
	if( src_lp>=tgt_ln+2 && isdigit(*(src+src_lp-tgt_ln-1)) && !memcmp( src+src_lp-tgt_ln, target, tgt_ln ) ){
		*(src+src_lp-tgt_ln) = '\0';
		strcat( stock, add_char );
		return 0;
	}
	return -1;
}


// 正規化の一環として番組名の余分な装飾を除去か概要に移動
void rep_flg_title( char *src )
{
	static char		*mvflg[] =		{	"全",           "新",           "最終回",   "終",     "完",
									    "日本語吹替版", "二／吹",       "二",       "ニ／吹", "ニ",
										"日本語字幕版", "字幕スーパー", "字幕版",   "字幕",   "字",
										"再",           "プレイバック", "無料放送", "無料",   "無",
										"吹",           "5.1",          "5．1",     "映画",   "映",
										"未放送話",
										"先行放送",     "先行",         "全話一挙放送", "一挙放送", "一挙",   "連日", "最新作",
										"デジタルリマスターHD版", "デジタルリマスター版", "デジタルリマスター", "日", "セレクション", "集中",
										"HDリマスター版","リマスター版","HDリマスター","HD版","HD","リマスター",
										"帯", "セレクト", "心理サスペンス", "サスペンス", "ミステリー",
										"リーガル", "スパイアクション", "スパイ", "アクション", "コメディ",
										"SFヒューマン", "医療ミステリー", "犯罪捜査", "法医学サスペンス", "サバイバル" };
	static int		flg_len[] =		{	 3,  3,  9,  3,  3,
										18,  9,  3,  9,  3,
										18, 18,  9,  6,  3,
										 3, 18, 12,  6,  3,
										 3,  3,  5,  6,  3,
										12,
										12,  6, 18, 12,  6,  6,  9,
										32, 30, 27,  3, 18,  6,
										20, 18, 17,  5,  2, 15,
										 3, 12, 21, 15, 15,
										12, 24,  9, 15, 12,
										17, 21, 12, 24, 15 };
	static char		*mvflgchg[] =	{	"(全",    "【新】",  "【終】",  "【終】", "【終】",
										"【吹】", "【二】",  "【二】",  "【二】", "【二】",
										"【字】", "【字】",  "【字】",  "【字】", "【字】",
										"【再】", "【再】",  "【無】",  "【無】", "【無】",
										"【吹】", "【5.1】", "【5.1】", "【映】", "【映】",
										"(未放送話)",
										"",       "",        "",        "",       "","","",
										"",       "",        "",        "",       "","",
										"",       "",        "",        "",       "","",
										"",       "",        "",        "",       "",
										"",       "",        "",        "",       "",
										"",       "",        "",        "",       ""};
	static char		*mvflgcls[] =	{ "回", "話" };
	static char		*mvflgclschg[] ={ "回)", "話)" };
	static char		*mv_flg_mkf[] =	{ "(", "【", "＜", "《", "<", "[", "-", " ", " ",  " ",  " " };
	static char		*mv_flg_mkb[] =	{ ")", "】", "＞", "》", ">", "]", "-", " ", "！", "!", "\0" };
	static int		mkf_len[] =		{   1,    3,    3,    3,   1,   1,   1,   1,    1,   1,    1 };
	static int		mkb_len[] = 	{   1,    3,    3,    3,   1,   1,   1,   1,    3,   1,    0 };
	int				lp;
	int				cls_lp;
	int				mk_c;
	char			*wk_pnt;
	char			*tmp_pnt;
	char			*jp_pnt;

	wk_pnt = tmp_pnt = src;
	while( *tmp_pnt ){
		if( *tmp_pnt != '\n' )
			*wk_pnt++ = *tmp_pnt;
		tmp_pnt++;
	}
	*wk_pnt = '\0';
	if( memcmp( src, "特:", 4 ) == 0 )
		memmove( src, src+4, strlen( src+4 )+1 );

	if( memcmp( src, "無料≫", 9 ) == 0 ){
		memmove( src, src+9, strlen( src+9 )+1 );
		strcat( stock, "【無】" );
	}
	if( memcmp( src, "☆NEW☆", 9 ) == 0 ){
		memmove( src, src+9, strlen( src+9 )+1 );
		strcat( stock, "【新】" );
	}
	if( memcmp( src, "5．1", 5 ) == 0 ){
		memmove( src, src+5, strlen( src+5 )+1 );
		strcat( stock, "【5.1】" );
	}
	if( memcmp( src+3, "◆", 3 ) == 0 )
		memcpy( src+3, "≫", 3 );

	// "全nn話"
	tmp_pnt = src;
	mk_c = 0;
	do{
		jp_pnt = src;
		if( wk_pnt=strstr( jp_pnt, mv_flg_mkf[mk_c] ), wk_pnt!=NULL ){
			tmp_pnt = wk_pnt + mkf_len[mk_c];
			while( !memcmp( tmp_pnt, mvflg[0], 3 ) ){
				tmp_pnt += 3;
				if( isdigit( *tmp_pnt ) ){
					char  buf[6];
					int   sc = 0;
					do{
						buf[sc++] = *tmp_pnt++;
					}while( isdigit( *tmp_pnt ) );
					buf[sc] = '\0';
					for( cls_lp=0; cls_lp<sizeof(mvflgcls)/sizeof(char *); cls_lp++ ){
						if( memcmp( tmp_pnt, mvflgcls[cls_lp], strlen( mvflgcls[cls_lp] ) ) == 0 ){
							strcat( stock, mvflgchg[0] );
							strcat( stock, buf );
							strcat( stock, mvflgclschg[cls_lp] );
							tmp_pnt += strlen( mvflgcls[cls_lp] );
							if( !memcmp( tmp_pnt, mv_flg_mkb[mk_c], mkb_len[mk_c] ) ){
								tmp_pnt += mkb_len[mk_c];
								if( *tmp_pnt==' ' && wk_pnt>src && *(wk_pnt-1)==' ' )
									tmp_pnt++;
								memmove( wk_pnt, tmp_pnt, strlen( tmp_pnt )+1 );
							}else
								if( *tmp_pnt==' ' || !memcmp( tmp_pnt, "／", 3 ) ){
									wk_pnt  += mkf_len[mk_c];
									tmp_pnt += *tmp_pnt==' ' ? 1 : 3;
									memmove( wk_pnt, tmp_pnt, strlen( tmp_pnt )+1 );
								}
							goto STAGE1;
						}
					}
				}
			}
		}
	}while( ++mk_c < sizeof(mv_flg_mkf)/sizeof(char *) );
STAGE1:

	wk_pnt = src;
	jp_pnt = src;
RE_START:
	mk_c = 0;
	do{
		if( !memcmp( wk_pnt, mv_flg_mkf[mk_c], mkf_len[mk_c] ) ){
			tmp_pnt = wk_pnt + mkf_len[mk_c];
			lp      = mk_c<sizeof(mv_flg_mkf)/sizeof(char *)-2 ? 1 : 2;
			do{
				if( !memcmp( tmp_pnt, mvflg[lp], flg_len[lp] ) ){
					tmp_pnt += flg_len[lp];
					if( !mkb_len[mk_c] ){
						if( *tmp_pnt == '\0' ){
							if( wk_pnt != src )
								*wk_pnt = '\0';
							goto STAGE2;
						}else{
							jp_pnt = wk_pnt = tmp_pnt;
							goto RE_START;
						}
					}else
						if( !memcmp( tmp_pnt, mv_flg_mkb[mk_c], mkb_len[mk_c] ) ){
							strcat( stock, mvflgchg[lp] );
							if( *mv_flg_mkf[mk_c] != ' ' ){
								wk_pnt = tmp_pnt + mkb_len[mk_c];
								while( *wk_pnt == ' ' )
									wk_pnt++;
							}else
								wk_pnt = tmp_pnt;
							goto RE_START;
						}else
							if( *tmp_pnt==' ' || !memcmp( tmp_pnt, "／", 3 ) ){
								strcat( stock, mvflgchg[lp] );
								tmp_pnt += *tmp_pnt==' ' ? 1 : 3;
								lp = 0;
							}else
								break;
				}
			}while( ++lp < sizeof(mvflg)/sizeof(char *) );
			if( wk_pnt != jp_pnt )
				memmove( jp_pnt, wk_pnt, strlen( wk_pnt )+1 );
			if( *mv_flg_mkf[mk_c] != ' ' ){
				if( tmp_pnt=strstr( jp_pnt, mv_flg_mkb[mk_c] ), tmp_pnt!=NULL ){
					jp_pnt = tmp_pnt + mkb_len[mk_c];
					if( *jp_pnt == ' ' )
						jp_pnt++;
					wk_pnt = jp_pnt;
					goto RE_START;
				}else
					goto STAGE2;
			}
		}
	}while( ++mk_c < sizeof(mv_flg_mkf)/sizeof(char *) );
	if( wk_pnt != jp_pnt )
		memmove( jp_pnt, wk_pnt, strlen( wk_pnt )+1 );

STAGE2:
	jp_pnt = src + strlen( src ) - 1;
	while( *jp_pnt == ' ' )
		*jp_pnt-- = '\0';
	mk_c = 0;
	do{
		jp_pnt = src;
RE_START2:
		if( wk_pnt=strstr( jp_pnt, mv_flg_mkf[mk_c] ), wk_pnt!=NULL ){
			tmp_pnt = wk_pnt + mkf_len[mk_c];
			if( ( !memcmp( tmp_pnt, "特集:", 7 ) || !memcmp( tmp_pnt, "出演:", 7 ) ) ){
				char	*chk_pnt;
				if( chk_pnt=strstr( tmp_pnt+7, mv_flg_mkb[mk_c] ), chk_pnt!=NULL ){
					chk_pnt += mkb_len[mk_c];
					if( *chk_pnt!='\0' && !( *mv_flg_mkb[mk_c]==' ' && !strcmp( chk_pnt, "他" ) ) ){
						strncat( stock, wk_pnt, chk_pnt-wk_pnt );
						if( src<wk_pnt && ( ALPHA_CHK( chk_pnt, wk_pnt-1 ) || ( *(wk_pnt-1)!=' ' && *chk_pnt!=' ' ) ) )
							*wk_pnt++ = ' ';
						memmove( wk_pnt, chk_pnt, strlen(chk_pnt)+1 );
						jp_pnt = wk_pnt;
						goto RE_START2;
					}
				}
				strcat( stock, wk_pnt );
				while( src<wk_pnt && *(wk_pnt-1)==' ' )
					wk_pnt--;
				*wk_pnt = '\0';
				continue;
			}
//			lp = 1;
			lp = mk_c<sizeof(mv_flg_mkf)/sizeof(char *)-2 ? 1 : 2;
			do{
				char	*stk_pnt;
				if( stk_pnt=strstr( tmp_pnt, mvflg[lp] ), stk_pnt!=NULL ){
					if( mkb_len[mk_c] == 0 ){
						if( tmp_pnt == stk_pnt ){
							tmp_pnt = stk_pnt + flg_len[lp];
							if( *tmp_pnt == '\0' ){
								if( wk_pnt != src ){
									strcat( stock, mvflgchg[lp] );
									*wk_pnt = '\0';
								}
								goto FIRST_LOOP2;
							}else{
								if( *wk_pnt==' ' && ( *tmp_pnt=='#' || flg_len[lp]>4 ) ){
									strcat( stock, mvflgchg[lp] );
									memmove( wk_pnt, tmp_pnt, strlen( tmp_pnt )+1 );
									jp_pnt = wk_pnt;
								}else
									jp_pnt = tmp_pnt;
								goto RE_START2;
							}
						}
					}else{
						char	*out_pnt = strstr( tmp_pnt, mv_flg_mkb[mk_c] );
						if( ( tmp_pnt<stk_pnt && ( out_pnt==NULL || out_pnt<stk_pnt ) ) || ( *wk_pnt==' ' && tmp_pnt!=stk_pnt ) ){
							continue;
						}
						char	*in_pnt = tmp_pnt;
						tmp_pnt = stk_pnt + flg_len[lp];
						if( tmp_pnt == out_pnt ){
							strcat( stock, mvflgchg[lp] );
							tmp_pnt += mkb_len[mk_c];
							while( *tmp_pnt == ' ' )
								tmp_pnt++;
							if( *tmp_pnt != '\0' ){
								if( src<wk_pnt && *(wk_pnt-1)!=' ' )
									*wk_pnt++ = ' ';
								memmove( wk_pnt, tmp_pnt, strlen( tmp_pnt )+1 );
								jp_pnt = wk_pnt;
								goto RE_START2;
							}else{
								while( src<wk_pnt && *(wk_pnt-1)==' ' )
									wk_pnt--;
								*wk_pnt = '\0';
								goto FIRST_LOOP2;
							}
						}else{
							if( in_pnt == stk_pnt ){
								if( *tmp_pnt==' ' || !memcmp( tmp_pnt, "／", 3 ) ){
									tmp_pnt += *tmp_pnt==' ' ? 1 : 3;
									strcat( stock, mvflgchg[lp] );
									lp = mk_c==0 ? 1 : 0;
								}else
									if( out_pnt != NULL ){
										jp_pnt = out_pnt + mkb_len[mk_c];
										goto RE_START2;
									}else
										goto FIRST_LOOP2;
							}
						}
					}
				}
			}while( ++lp < sizeof(mvflg)/sizeof(char *) );
			if( *mv_flg_mkf[mk_c] != ' ' ){
				if( tmp_pnt=strstr( tmp_pnt, mv_flg_mkb[mk_c] ), tmp_pnt!=NULL ){
					jp_pnt = tmp_pnt + mkb_len[mk_c];
					if( *jp_pnt == ' ' )
						jp_pnt++;
					goto RE_START2;
				}
			}
		}
FIRST_LOOP2:;
	}while( ++mk_c < sizeof(mv_flg_mkf)/sizeof(char *) );

	if( tmp_pnt=strstr( src, "話一挙放送" ), tmp_pnt!=NULL ){
		if( strlen( tmp_pnt ) == 15 ){
			if( tmp_pnt=strstr( src, "全" ), tmp_pnt!=NULL ){
				jp_pnt = tmp_pnt +3;
				if( isdigit( *jp_pnt ) ){
					char  buf[6];
					int   sc = 0;
					do{
						buf[sc++] = *jp_pnt++;
					}while( isdigit( *jp_pnt ) );
					buf[sc] = '\0';
					if( strncmp( src, "「", 3 )==0 && strncmp( tmp_pnt-3, "」", 3 )==0 ){
						memmove( src, src+3, strlen(src+3) );
						tmp_pnt -= 6;
					}
					memcpy( tmp_pnt, " #01-", 5 );
					memcpy( tmp_pnt+5, buf, sc+1 );
					return;
				}
			}
		}
	}
STAGE3:
	if( ev_flg_move( src, "(新)", "【新】" ) == 0 )
		goto STAGE3;
	if( ev_flg_move( src, " 新", "【新】" ) == 0 )
		goto STAGE3;
	if( ev_flg_move( src, " 終", "【終】" ) == 0 )
		goto STAGE3;
	if( ev_flg_move( src, "完", "【終】" ) == 0 )
		goto STAGE3;
	if( ev_flg_move( src, " END", "【終】" ) == 0 )
		goto STAGE3;
}


void rep_flg_desc( char *src )
{
	char	*tmp_pnt;
	char	*wk_pnt;

	if( !memcmp( src, "最終話", 9 ) ){
		wk_pnt = src + 9;
		memmove( src, wk_pnt, strlen( wk_pnt )+1 );
		if( strstr( src, "【終】" )==NULL && strstr( stock, "【終】" )==NULL )
			strcat( stock, "【終】" );
		return;
	}
	if( tmp_pnt=strstr( src, "(終)" ), tmp_pnt!=NULL ){
		wk_pnt = tmp_pnt + 5;
		memmove( tmp_pnt, wk_pnt, strlen( wk_pnt )+1 );
		if( strstr( src, "【終】" )==NULL && strstr( stock, "【終】" )==NULL )
			strcat( stock, "【終】" );
		return;
	}
	if( tmp_pnt=strstr( src, "(完)" ), tmp_pnt!=NULL ){
		wk_pnt = tmp_pnt + 5;
		memmove( tmp_pnt, wk_pnt, strlen( wk_pnt )+1 );
		if( strstr( src, "【終】" )==NULL && strstr( stock, "【終】" )==NULL )
			strcat( stock, "【終】" );
	}
}
#endif


int parseEIThead(unsigned char *data, EIThead *h) {
	int boff = 0;

	memset(h, 0, sizeof(EIThead));

	h->table_id = getBit(data, &boff, 8);
	h->section_syntax_indicator = getBit(data, &boff, 1);
	h->reserved_future_use = getBit(data, &boff, 1);
	h->reserved1 = getBit(data, &boff, 2);
	h->section_length =getBit(data, &boff,12);
	h->service_id = getBit(data, &boff, 16);
	h->reserved2 = getBit(data, &boff, 2);
	h->version_number = getBit(data, &boff, 5);
	h->current_next_indicator = getBit(data, &boff, 1);
	h->section_number = getBit(data, &boff, 8);
	h->last_section_number = getBit(data, &boff, 8);
	h->transport_stream_id = getBit(data, &boff, 16);
	h->original_network_id = getBit(data, &boff, 16);
	h->segment_last_section_number = getBit(data, &boff, 8);
	h->last_table_id = getBit(data, &boff, 8);
  
	return 14;
}

int parseEITbody(unsigned char *data, EITbody *b, int section_number )
{
	int				tnum;
	int				boff = 0;
	unsigned char	start_time[5];
	unsigned char	duration[3];

	memset(b, 0, sizeof(EITbody));

	b->event_id = getBit(data, &boff, 16);

	memcpy(start_time, data + boff / 8, 5);
	boff += 8 * 5;
	memcpy(duration, data + boff / 8, 3);
	boff += 8 * 3;
	b->running_status = getBit(data, &boff, 3);
	b->free_CA_mode = getBit(data, &boff, 1);
	b->descriptors_loop_length = getBit(data, &boff, 12);

	/* 日付変換 */
	if( start_time[0]==0xFFU && start_time[1]==0xFFU && start_time[2]==0xFFU && start_time[3]==0xFFU && start_time[4]==0xFFU ){
		b->event_status |= START_TIME_UNCERTAINTY;
		b->yy = 138;
		b->mm = section_number + 1;
	}else{
		tnum = (unsigned int)start_time[0] << 8 | (unsigned int)start_time[1];

		b->yy = (tnum - 15078.2) / 365.25;
		b->mm = ((tnum - 14956.1) - (int)(b->yy * 365.25)) / 30.6001;
		b->dd = (tnum - 14956) - (int)(b->yy * 365.25) - (int)(b->mm * 30.6001);

		if(b->mm == 14 || b->mm == 15) {
			b->yy += 1;
			b->mm = b->mm - 1 - (1 * 12);
		} else {
			b->mm = b->mm - 1;
		}

		b->hh = (int)BCD(start_time[2]);
		b->hm = (int)BCD(start_time[3]);
		b->ss = (int)BCD(start_time[4]);
	}

	if( chkBCD(duration[0]) && chkBCD60(duration[1]) && chkBCD60(duration[2]) ){
		b->duration = (int)BCD(duration[0])*3600 + (int)BCD(duration[1])*60 + (int)BCD(duration[2]);
	}else{
		b->event_status |= DURATION_UNCERTAINTY;
	}
	return 12;
}

//短形式イベント
int parseSEVTdesc(unsigned char *data, SEVTdesc *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(SEVTdesc));

	desc->descriptor_tag = getBit(data, &boff, 8);
	if((desc->descriptor_tag & 0xFF) != 0x4D) {
		return -1;
	}
	desc->descriptor_length = getBit(data, &boff, 8);
	memcpy(desc->ISO_639_language_code, data + boff / 8, 3);
	/* desc->ISO_639_language_code = getBit(data, &boff, 24); */
	boff += 24;
	desc->event_name_length = getBit(data, &boff, 8);
#if MoveAribMark
	memset(stock, 0, sizeof stock );
	treat_mode = ON;
	stock_mode = ON;
#endif
	getStr(desc->event_name, data, &boff, desc->event_name_length);
#if MoveAribMark
	rep_flg_title( desc->event_name );
	stock_mode = OFF;
#endif
	desc->text_length = getBit(data, &boff, 8);
	getStr(desc->text, data, &boff, desc->text_length);
#if MoveAribMark
	rep_flg_desc( desc->text );
	strcat( desc->text, stock );
//	*stock = '\0';	
	treat_mode = OFF;
#endif

	return desc->descriptor_length + 2;
}

//コンテント記述子
int parseContentDesc(unsigned char *data, ContentDesc *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(ContentDesc));

	desc->descriptor_tag = getBit(data, &boff, 8);
	if((desc->descriptor_tag & 0xFF) != 0x54) {
		return -1;
	}
	desc->descriptor_length = getBit(data, &boff, 8);
	memcpy(desc->content, data+(boff/8), desc->descriptor_length);
	//getStr(desc->content, data, &boff, desc->descriptor_length);
	return desc->descriptor_length + 2;
}

//シリーズ記述子
int parseSeriesDesc(unsigned char *data, SeriesDesc *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(SeriesDesc));

	desc->descriptor_tag = getBit(data, &boff, 8);
	if((desc->descriptor_tag & 0xFF) != 0xD5) {
		return -1;
	}
	desc->descriptor_length = getBit(data, &boff, 8);
	desc->series_id = getBit(data, &boff, 16);
	desc->repeat_label = getBit(data, &boff, 4);
	desc->program_pattern = getBit(data, &boff, 3);
	desc->expire_date_valid_flag = getBit(data, &boff, 1);

	desc->expire_date = getBit(data, &boff, 16);
	//memcpy(desc->expire_date, data + boff / 8, 2);
	//boff += 16;

	desc->episode_number = getBit(data, &boff, 12);
	desc->last_episode_number = getBit(data, &boff, 12);

	getStr(desc->series_name_char, data, &boff, desc->descriptor_length - 8);
	return desc->descriptor_length + 2;
}

int parseComponentDesc(unsigned char *data, ComponentDesc *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(ComponentDesc));

	desc->descriptor_tag = getBit(data, &boff, 8);
	if((desc->descriptor_tag & 0xFF) != 0x50) {
		return -1;
	}
	desc->descriptor_length = getBit(data, &boff, 8);
	desc->reserved_future_use = getBit(data, &boff, 4);
	desc->stream_content = getBit(data, &boff, 4);
	desc->component_type = getBit(data, &boff, 8);
	desc->component_tag = getBit(data, &boff, 8);
	memcpy(desc->ISO_639_language_code, data + boff / 8, 3);
	boff += 24;
	getStr(desc->text_char, data, &boff, desc->descriptor_length - 6);
	return desc->descriptor_length + 2;
}

int parseAudioComponentDesc(unsigned char *data, AudioComponentDesc *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(AudioComponentDesc));

	desc->descriptor_tag = getBit(data, &boff, 8);
	if((desc->descriptor_tag & 0xFF) != 0xC4) {
		return -1;
	}
	desc->descriptor_length = getBit(data, &boff, 8);
	desc->reserved_future_use_1 = getBit(data, &boff, 4);
	desc->stream_content = getBit(data, &boff, 4);
	desc->component_type = getBit(data, &boff, 8);
	desc->component_tag = getBit(data, &boff, 8);
	desc->stream_type = getBit(data, &boff, 8);
	desc->simulcast_group_tag = getBit(data, &boff, 8);
	desc->ES_multi_lingual_flag = getBit(data, &boff, 1);
	desc->main_component_flag = getBit(data, &boff, 1);
	desc->quality_indicator = getBit(data, &boff, 2);
	desc->sampling_rate = getBit(data, &boff, 3);
	desc->reserved_future_use_2 = getBit(data, &boff, 1);
	memcpy(desc->ISO_639_language_code_1, data + boff / 8, 3);
	boff += 24;
	memcpy(desc->ISO_639_language_code_2, data + boff / 8, 3);
	boff += 24;
	getStr(desc->text_char, data, &boff, desc->descriptor_length - desc->ES_multi_lingual_flag ? 12 : 9);
	return desc->descriptor_length + 2;
}

#if REC10
char* parseComponentDescType(int componentDescType) {
	static char str[MAXSECLEN];
	memset(str, '\0', sizeof(str));
	char *strpart;

	switch (componentDescType & 0xF0) {
	case 0x00 :
		strpart = "映像480i ";
		break;
	case 0x90 :
		strpart = "映像2160p ";
		break;
	case 0xA0 :
		strpart = "映像480p ";
		break;
	case 0xB0 :
		strpart = "映像1080i ";
		break;
	case 0xC0 :
		strpart = "映像720p ";
		break;
	case 0xD0 :
		strpart = "映像240p ";
		break;
	case 0xE0 :
		strpart = "映像1080p ";
		break;
	default :
		strpart = "映像不明 ";
		break;
	}
	strcat(str, strpart);

	switch (componentDescType & 0x0F) {
	case 0x01 :
		strpart = "アスペクト比4:3";
		break;
	case 0x02 :
		strpart = "アスペクト比16:9 パンベクトルあり";
		break;
	case 0x03 :
		strpart = "アスペクト比16:9 パンベクトルなし";
		break;
	case 0x04 :
		strpart = "アスペクト比 > 16:9";
		break;
	default :
		strpart = "アスペクト比不明";
		break;
	}
	strcat(str, strpart);

	return str;
}

char* parseAudioComponentDescType(int AudiocomponentDescType) {
	static char str[MAXSECLEN];
	memset(str, '\0', sizeof(str));
	char *strpart;

	switch (AudiocomponentDescType) {
	case 0x01 :
		strpart = "音声1/0モード(シングルモノ)";
		break;
	case 0x02 :
		strpart = "音声1/0+1/0モード(デュアルモノ)";
		break;
	case 0x03 :
		strpart = "音声2/0モード(ステレオ)";
		break;
	case 0x04 :
		strpart = "音声2/1モード";
		break;
	case 0x05 :
		strpart = "音声3/0モード";
		break;
	case 0x06 :
		strpart = "音声2/2モード";
		break;
	case 0x07 :
		strpart = "音声3/1モード";
		break;
	case 0x08 :
		strpart = "音声3/2モード";
		break;
	case 0x09 :
		strpart = "音声3/2+LFEモード(3/2.1モード)";
		break;
	case 0x0A :
		strpart = "音声3/3.1モード";
		break;
	case 0x0B :
		strpart = "音声2/0/0-2/0/2-0.1モード";
		break;
	case 0x0C :
		strpart = "音声5/2.1モード";
		break;
	case 0x0D :
		strpart = "音声3/2/2.1モード";
		break;
	case 0x0E :
		strpart = "音声2/0/0-3/0/2-0.1モード";
		break;
	case 0x0F :
		strpart = "音声0/2/0-3/0/2-0.1モード";
		break;
	case 0x10 :
		strpart = "音声2/0/0-3/2/3-0.2モード";
		break;
	case 0x11 :
		strpart = "音声3/3/3-5/2/3-3/0/0.2モード";
		break;
	case 0x40 :
		strpart = "音声視覚障害者用解説";
		break;
	case 0x41 :
		strpart = "音声聴覚障害者用";
		break;
	default :
		strpart = "音声不明";
		break;
	}
	strcat(str, strpart);

	return str;
}
#endif

//拡張形式イベント
int parseEEVTDhead(unsigned char *data, EEVTDhead *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(EEVTDhead));

	desc->descriptor_tag = getBit(data, &boff, 8);
	if((desc->descriptor_tag & 0xFF) != 0x4E) {
		return -1;
	}
	desc->descriptor_length = getBit(data, &boff, 8);
	desc->descriptor_number = getBit(data, &boff, 4);
	desc->last_descriptor_number = getBit(data, &boff, 4);
	memcpy(desc->ISO_639_language_code, data + boff / 8, 3);
	/* desc->ISO_639_language_code = getBit(data, &boff, 24); */
	boff += 24;

	desc->length_of_items = getBit(data, &boff, 8);

	return 7;
}

int parseEEVTDitem(unsigned char *data, EEVTDitem *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(EEVTDitem));

	desc->item_description_length = getBit(data, &boff, 8);
	memset(desc->item_description, 0, MAXSECLEN);
	getStr(desc->item_description, data, &boff, desc->item_description_length);

	desc->item_length = getBit(data, &boff, 8);
//	memset(desc->item, 0, MAXSECLEN);
	memcpy(desc->item, data + (boff / 8), desc->item_length);
	desc->item[desc->item_length] = '\0';
	/* getStr(desc->item, data, &boff, desc->item_length); */

	return desc->item_description_length + desc->item_length + 2;
}

int parseEEVTDtail(unsigned char *data, EEVTDtail *desc) {
	int boff = 0;
  
	memset(desc, 0, sizeof(EEVTDtail));

	desc->text_length = getBit(data, &boff, 8);
	memset(desc->text, 0, MAXSECLEN);
	getStr(desc->text, data, &boff, desc->text_length);

	return desc->text_length + 1;
}

int checkEEVTDitem(EEVTDitem *save, EEVTDitem *new, int descriptor_number) {

	EEVTDitem swap;
	int boff = 0;

	if(new == NULL) {
		if(save->item_length != 0) {
			swap = *save;
			memset(save->item, 0, MAXSECLEN);
			getStr(save->item, (unsigned char*)swap.item, &boff, swap.item_length);
			return 1;
		} else {
			return 0;
		}
	}

	if(new->item_description_length == 0) {
		/* 続き 保存 */
		memcpy(save->item + save->item_length, new->item, new->item_length);
		save->item_length += new->item_length;
		return 0;
	} else {
		/* ブレーク。saveを印刷対象にする。saveをクリア? */
		if(save->item_length != 0) {
			/* 退避済みがあり */
			swap = *save;
			memset(save->item, 0, MAXSECLEN);
			getStr(save->item, (unsigned char*)swap.item, &boff, swap.item_length);
			swap = *new;
			*new = *save;
			*save = swap;
			save->descriptor_number = descriptor_number;
		} else {
			*save = *new;
			save->descriptor_number = descriptor_number;
			return 0;
		}
	}

	return 1;
}
EIT_CONTROL	*searcheit(EIT_CONTROL *top, int servid, int eventid)
{
	EIT_CONTROL	*cur ;
	cur = top ;

	while(cur != NULL){
		if((cur->event_id == eventid) && (cur->servid == servid)){
			return cur ;
		}

		cur = cur->next ;
	}
	return NULL ;
}


EIT_NULLSEGMENT *top_eitnull = NULL;


EIT_NULLSEGMENT *search_eitnull( int service_id, int table_id, int section_number, int version_number )
{
	EIT_NULLSEGMENT *cur;

	if( top_eitnull == NULL ){
		top_eitnull             = calloc( 1, sizeof(EIT_NULLSEGMENT) );
		top_eitnull->service_id = 0;
		top_eitnull->prev       = NULL;
		cur                     = top_eitnull;
	}else{
		cur = top_eitnull;
		while( cur->next != NULL ){
			cur = cur->next;
			if( cur->service_id==service_id && cur->table_id==table_id && cur->section_number==section_number )
				return cur;
		}
	}
	if( version_number != -1 ){
// printf( "EIT NULL SET: sv:%d tbl:%xH sec:%03d(%02dH) ver:%d\n", service_id, table_id, section_number, section_number/8*3, version_number );
		cur->next                 = calloc( 1, sizeof(EIT_NULLSEGMENT) );
		cur->next->prev           = cur;
		cur->next->next           = NULL;
		cur->next->service_id     = service_id;
		cur->next->table_id       = table_id;
		cur->next->section_number = section_number;
		cur->next->version_number = version_number;
	}
	return NULL;
}


void set_eitnull( int service_id, int table_id, int section_number, int version_number )
{
	EIT_NULLSEGMENT *eit_null = search_eitnull( service_id, table_id, section_number, version_number );

	if( eit_null != NULL ){
		if( eit_null->version_number != version_number ){
			// バージョン更新につき破棄処理
			eit_null->version_number = version_number;
			for( int tbl_loop=(table_id&0xF0); tbl_loop<16; tbl_loop++ ){
				for( int sec_loop=0; sec_loop<8; sec_loop++ ){
					if( !( tbl_loop==table_id && sec_loop==section_number ) ){
						eit_null = search_eitnull( service_id, tbl_loop, sec_loop, version_number );
/*
						if( eit_null->version_number != version_number ){
							if( eit_null->prev != NULL )
								eit_null->prev->next = eit_null->next;
							if( eit_null->next != NULL )
								eit_null->next->prev = eit_null->prev;
							free( eit_null );
						}
*/
					}
				}
			}
		}
	}
}


#if REC10
void append_desc(EIT_CONTROL* eittop, int service_id, int event_id, EEVTDitem* eevtitem) {
	EIT_CONTROL *cur;
	int str_alen = 0;

	cur = searcheit(eittop, service_id, event_id);
	if (cur == NULL) {
		return;
	}

	if ( cur->desc ) {
		str_alen = strlen( cur->desc );
	}
	else {
		str_alen = 0;
	}
	//eevtitem->item_description_length = strlen(eevtitem->item_description);
	//eevtitem->item_length = strlen(eevtitem->item);
	cur->desc = realloc(cur->desc, str_alen + eevtitem->item_description_length + eevtitem->item_length + 1000);
	if ( !str_alen ) *cur->desc = '\0';

	if ( eevtitem->item_description_length && !strstr(cur->desc, eevtitem->item_description) ) {
		strcat(cur->desc + str_alen, eevtitem->item_description);
		strcat(cur->desc, "\t");
	}

	if ( eevtitem->item_length && !strstr(cur->desc, eevtitem->item) ) {
		strcat(cur->desc + str_alen, eevtitem->item);
	//printf("%s\n",eevtitem->item);
		strcat(cur->desc, "\\n");
	}
}
#endif


char	*strstr_eucjp(const char *str, const char *search)
{
	char *pos ;
	pos = (char *)str ;

	while (*pos != '\0') {
		if (*pos == *search) {
			if (strncmp(pos, search, strlen(search)) == 0) {
				return pos ;
			}
		}
		if (*(unsigned char *)pos == 0x8Fu) {
			pos += 3 ;
		} else if (*(unsigned char *)pos >= 0x80u) {
			pos += 2 ;
		} else {
			pos += 1 ;
		}
	}

	return NULL ;
}


#ifndef KATAUNA
void	conv_title_subtitle(EIT_CONTROL *eitptr)
{
	int		lp = 0 ;
//	size_t	addsize ;
	char	*ptr ;
//	char	*ptr2 ;
	char	*newsubtitle ;

	for(lp = 0 ; subtitle_cnv_str[lp] != NULL ; lp++){
		ptr = strstr(eitptr->title, subtitle_cnv_str[lp]);
		if(ptr == NULL){
			continue ;
		}
		// タイトルがなくならないように
		if(ptr == eitptr->title){
			continue ;
		}
/*		ptr2 = ptr ;
		for( ; (unsigned char)*ptr2 == 0x20u ; ptr2++ );
		for( ; (unsigned char)*ptr2 == 0xA1u && (unsigned char)*(ptr2+1) == 0xA1u ; ptr2 += 2);
		for( ; (unsigned char)*ptr2 == 0x20u ; ptr2++ );
		newsubtitle = calloc(1, ((strlen(ptr2) + 2) + (strlen(eitptr->subtitle) + 1)));
		memcpy(newsubtitle, ptr2, strlen(ptr2));
//		*(newsubtitle+strlen(ptr)) = ' ';
		strcat(newsubtitle, "▽");
*/
		newsubtitle = calloc(1, ((strlen(ptr) + 1) + (strlen(eitptr->subtitle) + 1)));
		memcpy(newsubtitle, ptr, strlen(ptr));
		newsubtitle[strlen(ptr)] = ' ';

		*ptr = '\0';
		strcat(newsubtitle, eitptr->subtitle);
		free(eitptr->subtitle);
		eitptr->subtitle = newsubtitle ;
		return ;
	}
}
#endif


void	enqueue(EIT_CONTROL *top, EIT_CONTROL *eitptr, SVT_CONTROL *svtcur)
{
	EIT_CONTROL	*cur ;
	EIT_CONTROL	*tmp;
	int		rc ;
	int		duration = 0;
	time_t	end_tm;

	if(top->next == NULL){
#if 0
		if( eitptr->table_id!=0x4EU && eitptr->table_id!=0x4FU ){
			svtcur->start_eit = eitptr;
			svtcur->start_eid = eitptr->event_id;
			svtcur->start_sid = eitptr->servid;
		}
#endif
		top->next    = eitptr ;
		eitptr->prev = top ;
		return ;
	}
	cur = top->next ;
	while(cur != NULL){
		rc = memcmp(&cur->yy, &eitptr->yy, (sizeof(int) * 3));
		if(rc == 0){
			rc = memcmp(&cur->hh, &eitptr->hh, (sizeof(int) * 3));
			if(rc == 0){
				cur->prev->next = eitptr;
				eitptr->prev    = cur->prev;
				if( cur->next != NULL ){
					//重複イベント削除
					while(1){
						duration += cur->duration;
						tmp       = cur->next;
						free( cur->title );
						free( cur->subtitle );
#if REC10
						if( cur->desc != NULL )
							free( cur->desc );
#endif
						free( cur );
						cur = tmp;
						if( cur != NULL ){
							if( duration >= eitptr->duration ){
								eitptr->next = cur;
								cur->prev    = eitptr;
								return;
							}
						}else{
							eitptr->next = NULL;
							return;
						}
					}
				}
				eitptr->next = NULL;
				free( cur->title );
				free( cur->subtitle );
#if REC10
				if( cur->desc != NULL )
					free( cur->desc );
#endif
				free( cur );
				return;
			}
		}
		if(rc > 0){
			cur->prev->next = eitptr;
			eitptr->prev    = cur->prev;
			end_tm = timeParse( eitptr ) + eitptr->duration;
			if( end_tm <= timeParse( cur ) ){
				cur->prev    = eitptr;
				eitptr->next = cur;
				return;
			}else{
				while(1){
					tmp = cur->next;
					free( cur->title );
					free( cur->subtitle );
#if REC10
					if( cur->desc != NULL )
						free( cur->desc );
#endif
					free( cur );
					cur = tmp;
					if( cur != NULL ){
						if( end_tm <= timeParse( cur ) ){
							cur->prev    = eitptr;
							eitptr->next = cur;
							return;
						}
					}else{
						eitptr->next = NULL;
						return;
					}
				}
			}
		}
		if( cur->next == NULL ){
			cur->next    = eitptr;
			eitptr->prev = cur;
			return;
		}
		cur = cur->next;
	}
	return ;

}


extern SVT_CONTROL *serachid(SVT_CONTROL *, int);
extern void enqueue_sdt(SVT_CONTROL *, SVT_CONTROL *);


void dumpEIT(unsigned char *ptr, SVT_CONTROL *svttop, int select_sid, int mode )
{

	EIThead  eith;
	EITbody  eitb;
	SEVTdesc sevtd;

	EEVTDhead eevthead;
	EEVTDitem eevtitem;
	EEVTDtail eevttail;

	EEVTDitem save_eevtitem;

	EIT_CONTROL	*cur;
	EIT_CONTROL	*eittop;
	SVT_CONTROL	*svtcur;

	int len = 0;
	int loop_len = 0;
	int loop_blen = 0;
	int loop_elen = 0;
	int eit_pf_flg;
#if ENABLE_1SEG
	int l_eit_flg = ( mode & 0x02 ) ? 1 : 0;

	mode &= 0x01;
#endif
	/* EIT */
	len = parseEIThead(ptr, &eith);
#if ENABLE_1SEG
	eit_pf_flg = ( l_eit_flg && eith.section_number<2 ) || ( !l_eit_flg && eith.table_id==0x4EU ) || eith.table_id==0x4FU ? 1 : 0;
#else
	eit_pf_flg = eith.table_id==0x4EU || eith.table_id==0x4FU ? 1 : 0;
#endif
	if( mode && !eit_pf_flg )
		return;

	/* EIT ヘッダから、どのSVTのEIT情報か特定する */
/*
	eittop = NULL;
	svtcur = svttop->next;
	while(1){
		if( svtcur ){
			if( eith.service_id == svtcur->service_id && eith.transport_stream_id == svtcur->transport_stream_id &&
														eith.original_network_id == svtcur->original_network_id ){
				eittop = eith.table_id!=0x4EU && eith.table_id!=0x4FU ? svtcur->eitsch : svtcur->eit_pf;
				break;
			}
			svtcur = svtcur->next;
		}else
			return;
	}
*/
	if( select_sid && select_sid!=eith.service_id )
		return;
	svtcur = serachid( svttop, eith.service_id );
	if( svtcur ){
		if( svtcur->import_stat < 0 )
			return;
	}else{
		svtcur = calloc(1, sizeof(SVT_CONTROL));
		svtcur->service_id  = eith.service_id;
		svtcur->eitsch      = calloc( 1, sizeof(EIT_CONTROL) );
		svtcur->eit_pf      = calloc( 1, sizeof(EIT_CONTROL) );
		svtcur->import_stat = 0;
		enqueue_sdt( svttop, svtcur );
	}
	eittop = !eit_pf_flg ? svtcur->eitsch : svtcur->eit_pf;

	ptr += len;
	loop_len = eith.section_length - (len - 3 + 4); // 3は共通ヘッダ長 4はCRC
	if( loop_len==0 && !eit_pf_flg ){
		// NULL EITの保存(放送休止の補完に使用)
		set_eitnull( svtcur->service_id, eith.table_id, eith.section_number, eith.version_number );
		return;
	}
	while(loop_len > 0) {
		/* 連続する拡張イベントは、漢字コードが泣き別れして
		   分割されるようだ。連続かどうかは、item_description_lengthが
		   設定されているかどうかで判断できるようだ。 */
		memset(&save_eevtitem, 0, sizeof(EEVTDitem));

		len = parseEITbody(ptr, &eitb, eith.section_number );
		ptr += len;
		loop_len -= len;
    
		/* printf("evtid:%d\n", eitb.event_id); */
    
		loop_blen = eitb.descriptors_loop_length;
		loop_len -= loop_blen;
		while(loop_blen > 0) {

			len = parseSEVTdesc(ptr, &sevtd);
			if(len > 0) {
				if( eitb.event_status != EVENT_UNCERTAINTY ){
					cur = searcheit(eittop, eith.service_id, eitb.event_id);
					if(cur == NULL){
						cur = calloc(1, sizeof(EIT_CONTROL));
						cur->table_id       = eith.table_id ;
						cur->event_id       = eitb.event_id ;
						cur->servid         = eith.service_id ;
						cur->version_number = eith.version_number;
						cur->section_number = eith.section_number;
						cur->last_section_number = eith.last_section_number;
						cur->segment_last_section_number = eith.segment_last_section_number;
						cur->running_status = eitb.running_status;
						cur->free_CA_mode   = eitb.free_CA_mode;
						cur->yy             = eitb.yy;
						cur->mm             = eitb.mm;
						cur->dd             = eitb.dd;
						cur->hh             = eitb.hh;
						cur->hm             = eitb.hm;
						cur->ss             = eitb.ss;
						cur->duration       = eitb.duration;
						cur->event_status   = eitb.event_status;
						cur->title = malloc( strlen(sevtd.event_name)+1 );
						memcpy( cur->title, sevtd.event_name, strlen(sevtd.event_name)+1 );
						cur->subtitle = malloc( strlen(sevtd.text) + 1 );
						memcpy( cur->subtitle, sevtd.text, strlen(sevtd.text)+1 );
						enqueue( eittop, cur, svtcur );
						if( !eit_pf_flg ){
							cur->import_cnt = svtcur->import_cnt++;
							cur->renew_cnt  = 0;
						}
					}else{
						cur->version_number = eith.version_number;
						cur->section_number = eith.section_number;
						cur->last_section_number = eith.last_section_number;
						cur->segment_last_section_number = eith.segment_last_section_number;
						cur->running_status = eitb.running_status;
						cur->yy             = eitb.yy;
						cur->mm             = eitb.mm;
						cur->dd             = eitb.dd;
						cur->hh             = eitb.hh;
						cur->hm             = eitb.hm;
						cur->ss             = eitb.ss;
						cur->duration       = eitb.duration;
						cur->event_status   = eitb.event_status;
						if( eit_pf_flg ){
							// eit[pf]
							EIT_CONTROL	*cur_tmp;

							if( eith.section_number == 0 ){
								if( svtcur->eit_pf->next != cur ){
									//eit[pf]の繰り上がり
									cur_tmp = svtcur->eit_pf->next;
									while( cur_tmp != NULL ){
										cur_tmp = free_EIT_CONTROL( cur_tmp );
										if( cur_tmp == cur )
											break;
									}
									svtcur->eit_pf->next = cur;
									cur->prev            = svtcur->eit_pf;
								}
							}
							if( eitb.event_status ){
								//上書での時系列連続性の判断が出来なくなるため以降を削除
								cur_tmp   = cur->next;
								cur->next = NULL;
								while( cur_tmp != NULL ){
									cur_tmp = free_EIT_CONTROL( cur_tmp );
								}
							}
						}else{
							cur->import_cnt = svtcur->import_cnt++;
							cur->renew_cnt++;
						}
					}
				}
			} else {
				len = parseEEVTDhead(ptr, &eevthead);

				/*
				  if(eith.service_id == 19304 && 
				  eitb.event_id == 46564) {
				  printf("aa");
				  }
				*/


				if(len > 0) {
					ptr += len;
					loop_blen -= len;

					loop_elen = eevthead.length_of_items;
					loop_len -= loop_elen;
					while(loop_elen > 0) {
						len = parseEEVTDitem(ptr, &eevtitem);

						ptr += len;
						loop_elen -= len;
						loop_blen -= len;

						if(checkEEVTDitem(&save_eevtitem, &eevtitem, 
										  eevthead.descriptor_number)) {
#if 0
							if(mode == 1) { /* long format */
								fprintf(out, "EEVT,%d,%d,%d,%s,%s\n", 
										eith.service_id,
										eitb.event_id,
										eevtitem.descriptor_number, /* 退避項目 */
										eevtitem.item_description,
										eevtitem.item);
							}
#endif
#if REC10
							append_desc(eittop, eith.service_id, eitb.event_id, &eevtitem);
#endif
						}
					}

					len = parseEEVTDtail(ptr, &eevttail);
#if 0
					if(mode == 1) { /* long format */
						fprintf(out, "EEVTt,%d,%d,%d,%s\n", 
								eith.service_id,
								eitb.event_id,
								eevthead.descriptor_number,
								eevttail.text);
					}
#endif
				} else {
					ContentDesc contentDesc;
					SeriesDesc			seriesDesc;
					ComponentDesc		componentDesc;
					AudioComponentDesc	audioComponentDesc;

					cur = searcheit(eittop, eith.service_id, eitb.event_id);

					switch (*ptr) {
					case 0x54 :
						len = parseContentDesc(ptr, &contentDesc);
						if (len > 0) {
							if(cur != NULL){
								unsigned char	*genre_p = (unsigned char *)contentDesc.content;

								cur->content_type    = genre_p[0] >> 4;
								if( cur->content_type != 14 )
									cur->content_subtype = genre_p[0] & 0x0fu;
								else
									cur->content_subtype = (genre_p[0]&0x0fu)==0x01u ? genre_p[1]+0x40u : genre_p[1];
								if( contentDesc.descriptor_length >= 4 ){
									cur->genre2 = genre_p[2] >> 4;
									if( cur->genre2 != 14 )
										cur->sub_genre2 = genre_p[2] & 0x0fu;
									else
										cur->sub_genre2 = (genre_p[2]&0x0fu)==0x01u ? genre_p[3]+0x40u : genre_p[3];
									if( contentDesc.descriptor_length >= 6 ){
										cur->genre3 = genre_p[4] >> 4;
										if( cur->genre3 != 14 )
											cur->sub_genre3 = genre_p[4] & 0x0fu;
										else
											cur->sub_genre3 = (genre_p[4]&0x0fu)==0x01u ? genre_p[5]+0x40u : genre_p[5];
									}else{
										cur->genre3     = 16;
										cur->sub_genre3 = 16;
									}
									if( cur->content_type == 14 ){
										int		sub_stock = cur->content_subtype;

										if( cur->genre2 != 14 ){
											cur->content_type    = cur->genre2;
											cur->content_subtype = cur->sub_genre2;
											cur->genre2          = 14;
											cur->sub_genre2      = sub_stock;
										}else
										if( cur->genre3!=14 && cur->genre3!=16 ){
											cur->content_type    = cur->genre3;
											cur->content_subtype = cur->sub_genre3;
											cur->genre3          = 14;
											cur->sub_genre3      = sub_stock;
										}
									}
								}else{
									cur->genre2     = 16;
									cur->sub_genre2 = 16;
									cur->genre3     = 16;
									cur->sub_genre3 = 16;
								}
							}
#if 0
								fprintf(stdout, "%s:", cur->title);
								fprintf(stdout, ",%02x%02x", (unsigned char)contentDesc.content[0], (unsigned char)contentDesc.content[1]);
								fprintf(stdout, ",%02x%02x", (unsigned char)contentDesc.content[2], (unsigned char)contentDesc.content[3]);
								fprintf(stdout, ",%02x%02x\n", (unsigned char)contentDesc.content[4], (unsigned char)contentDesc.content[5]);
#endif
						}
						break;
					case 0xD5 :
						len = parseSeriesDesc(ptr, &seriesDesc);
						if (len > 0) {
							if(cur != NULL)
								cur->episode_number = seriesDesc.episode_number;
							#if 0
								printf("Series,%d,%d,series=%d,repeat=%01x,pattern=%d,expire_valid=%d,expire=%04x,epinum=%d,lastepinum=%d,%s\n",
									eith.service_id,
									eitb.event_id,
									seriesDesc.series_id,
									seriesDesc.repeat_label,
									seriesDesc.program_pattern,
									seriesDesc.expire_date_valid_flag,
									seriesDesc.expire_date,
									seriesDesc.episode_number,
									seriesDesc.last_episode_number,
									seriesDesc.series_name_char);
							#endif
						}
						break;
					case 0x50 :
						len = parseComponentDesc(ptr, &componentDesc);
						if (len > 0) {
							if(cur != NULL){
								cur->video_type = componentDesc.component_type;
							}
							#if 0
							printf("Component,%d %d %s\n",
								componentDesc.stream_content, 
								componentDesc.component_type, 
								parseComponentDescType(componentDesc.component_type));
							#endif
						}
						break;
					case 0xC4 :
						len = parseAudioComponentDesc(ptr, &audioComponentDesc);
						if (len > 0) {
							if(cur != NULL){
								cur->audio_type = audioComponentDesc.component_type;
								cur->multi_type = audioComponentDesc.ES_multi_lingual_flag;
							}
							#if 0
							printf("AudioComponent,%d %d %d %s\n",
								audioComponentDesc.component_type, 
								audioComponentDesc.ES_multi_lingual_flag, 
								audioComponentDesc.sampling_rate, 
								parseAudioComponentDescType(audioComponentDesc.component_type)
								);
							#endif
						}
						break;
					default :
						len = parseOTHERdesc(ptr);
					}
				}
			}
			ptr += len;
			loop_blen -= len;
		}
		/* 最後のブレークチェック */
		if(checkEEVTDitem(&save_eevtitem, NULL, 0)) {
#if 0
			if(mode == 1) { /* long format */
				fprintf(out, "EEVT,%d,%d,%d,%s,%s\n", 
						eith.service_id,
						eitb.event_id,
						save_eevtitem.descriptor_number,
						save_eevtitem.item_description,
						save_eevtitem.item);
			}
#endif
#if REC10
			append_desc(eittop, eith.service_id, eitb.event_id, &save_eevtitem);
#endif
		}
	}

	return;
}
