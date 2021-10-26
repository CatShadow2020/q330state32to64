#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <libgen.h>
#include <stdint.h>
#include <string.h>
#include <linux/limits.h>
#include <vector>
#include <string>
#include <memory>
#include "util.h"

#define CTY_STATIC 0 /* Static storage for status, etc */
#define CTY_SYSTEM 1 /* system identification */
#define CTY_IIR 2 /* IIR filter */
#define CTY_FIR 3 /* FIR filter */
#define CTY_AVG 4 /* Averaging */
#define CTY_LCQ 5 /* A LCQ */
#define CTY_RING 6 /* Ring Buffer Entry */
#define CTY_MH 7 /* Murdock Hutt Detector */
#define CTY_THR 8
#define CTY_DPLCQ 9 /* DP LCQ */
#define CTY_PURGED 86 /* Continuity file already used */
#define DP_MESSAGE 0x7F /* when used as dp_src means message log */

typedef char tseed_net[2];
#define longint int32_t
#define word uint16_t
#define byte uint8_t
#define boolean uint8_t
#define integer int32_t
#define longword uint32_t
typedef char tseed_stn[5];
typedef longword t64[2];
typedef char string9[10];
#define single float
typedef void *pointer;
typedef longint taccminutes[60];
typedef byte tip_v6[16];
typedef char tlocation[2];
typedef char tseed_name[3];
typedef longint crc_table_type[256];
typedef double tfloat;

#define MAXMTU 576
#define BOOM_COUNT 6
#define SENS_CHANNELS 6
#define CRC_POLYNOMIAL 1443300200

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
} tctyhdr;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    byte version;
    byte high_freq; /* high frequency encoding */
    t64 serial; /* serial number of q330 */
    double lasttime; /* data_timetag of last second of data */
    word last_dataqual; /* 0-100% */
    longword last_dataseq; /* data record sequence. sequence continuity important too */
    longint comm_event_bitmask;
    longword reboot_counter; /* last reboot counter */
} tsystem;

typedef struct
{
    byte chan;
    byte seg_freq;
    word size;
} tdp_mult;

typedef tdp_mult *pdp_mult;

#pragma pack(4)
typedef struct
{
    uint32_t ppkt; /* this points to the following pkt */
    byte pkt[MAXMTU - 40];
} dholdqtype32;

#pragma pack(8)
typedef struct
{
    pdp_mult ppkt; /* this points to the following pkt */
    byte pkt[MAXMTU - 40];
} dholdqtype64;

#pragma pack(4)
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    byte version;
    byte high_freq; /* high frequency encoding */
    t64 serial; /* serial number of q330 */
    double lasttime; /* data_timetag of last second of data */
    word last_dataqual; /* 0-100% */
    longword last_dataseq; /* data record sequence. sequence continuity important too */
    longint comm_event_bitmask;
    longword reboot_counter; /* last reboot counter */
} tsystem32;

#pragma pack(8)
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    byte version;
    byte high_freq; /* high frequency encoding */
    t64 serial; /* serial number of q330 */
    double lasttime; /* data_timetag of last second of data */
    word last_dataqual; /* 0-100% */
    longword last_dataseq; /* data record sequence. sequence continuity important too */
    longint comm_event_bitmask;
    longword reboot_counter; /* last reboot counter */
} tsystem64;

#define FRAMES_PER_RECORD 8
#define WORDS_PER_FRAME 16
#define FRAME_SIZE (WORDS_PER_FRAME * 4)
#define LIB_REC_SIZE (FRAME_SIZE * FRAMES_PER_RECORD)

typedef byte completed_record[LIB_REC_SIZE];

#pragma pack(4)
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    word spare; /* for longword alignment */
    completed_record comprec;
} tctyring;

#pragma pack(8)
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    word spare; /* for longword alignment */
    completed_record comprec;
} tctyring64;

#pragma pack(4)

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    longword lastdtsequence; /* last data record sequence processed */
    dholdqtype32 multbuf; /* holding buffer for out-of-order DC_MULT packet */
    integer prev_rate; /* rate when continuity written */
    tfloat prev_delay; /* delay when continuity written */
#ifndef OMIT_SEED
    boolean glast; /* gen_last_on */
    boolean con; /* cal_on */
    boolean cstat; /* calstat */
    boolean overwrite_slipping;
    byte qpad;
    word cinc; /* calinc */
    longint rec_written; /* records written */
    longint arec_written; /* archive records written */
    longword gnext; /* gen_next */
    longint last_sample; /* last compression sample */
    double nextrec_tag; /* this is the expected starting time of the next record */
    double lastrec_tag; /* this was the last timetag used */
    double backup_timetag;
    word backup_timequal;
#endif
} tctylcq32;

#pragma pack(8)
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    longword lastdtsequence; /* last data record sequence processed */
    dholdqtype64 multbuf; /* holding buffer for out-of-order DC_MULT packet */
    integer prev_rate; /* rate when continuity written */
    tfloat prev_delay; /* delay when continuity written */
    boolean glast; /* gen_last_on */
    boolean con; /* cal_on */
    boolean cstat; /* calstat */
    boolean overwrite_slipping;
    byte qpad;
    word cinc; /* calinc */
    longint rec_written; /* records written */
    longint arec_written; /* archive records written */
    longword gnext; /* gen_next */
    longint last_sample; /* last compression sample */
    double nextrec_tag; /* this is the expected starting time of the next record */
    double lastrec_tag; /* this was the last timetag used */
    double backup_timetag;
    word backup_timequal;
} tctylcq64;

#define MAXSECTIONS 4
#define MAXPOLES 8
#define FILTER_NAME_LENGTH 31

#pragma pack(4)

typedef double tvector32[MAXPOLES + 1];

typedef struct
{
    tvector32 x;
    tvector32 y;
} tiirvalues32;
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    char fn[FILTER_NAME_LENGTH];
    tiirvalues32 flt[MAXSECTIONS + 1]; /* element 0 not used */
    tfloat outbuf; /* this may be an array */
} tctyiir32;

#pragma pack(8)

typedef double tvector64[MAXPOLES + 1];

typedef struct
{
    tvector64 x;
    tvector64 y;
} tiirvalues64;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    char fn[FILTER_NAME_LENGTH];
    tiirvalues64 flt[MAXSECTIONS + 1]; /* element 0 not used */
    tfloat outbuf; /* this may be an array */
} tctyiir64;

#define DETECTOR_NAME_LENGTH 31
#pragma pack(4)

typedef struct
{
    boolean detector_on; /* if last record was detected on */
    boolean detection_declared; /* detector_on filtered with first_detection */
    boolean first_detection; /* if this is first detection after startup */
    boolean detector_enabled; /* currently enabled */
    boolean new_onset; /* TRUE if new onset */
    boolean default_enabled; /* detector enabled by default */
    integer sampcnt; /* Number of samples in "insamps" so far */
    longint total_detections; /* used to keep track of detections over time */
    double startt; /* Starting time of first data point */
    double etime;
    longint peakhi; /*highest value of high limit*/
    longint peaklo; /*lowest value of low limit*/
    longint waitdly; /*sample countdown*/
    integer overhi; /*number of points over the high limit*/
    integer overlo; /*number of points under the low limit*/
    boolean hevon; /*high limit event on*/
    boolean levon; /*low limit event on*/
    tfloat sample_rate; /* sample rate in seconds */
    int32_t onsetdata;
    int32_t parent;
} threshold_control_struc32;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    char dn[DETECTOR_NAME_LENGTH];
    threshold_control_struc32 thr_cont;
} tctythr32;

#pragma pack(8)

typedef struct
{
    boolean detector_on; /* if last record was detected on */
    boolean detection_declared; /* detector_on filtered with first_detection */
    boolean first_detection; /* if this is first detection after startup */
    boolean detector_enabled; /* currently enabled */
    boolean new_onset; /* TRUE if new onset */
    boolean default_enabled; /* detector enabled by default */
    integer sampcnt; /* Number of samples in "insamps" so far */
    longint total_detections; /* used to keep track of detections over time */
    double startt; /* Starting time of first data point */
    double etime;
    longint peakhi; /*highest value of high limit*/
    longint peaklo; /*lowest value of low limit*/
    longint waitdly; /*sample countdown*/
    integer overhi; /*number of points over the high limit*/
    integer overlo; /*number of points under the low limit*/
    boolean hevon; /*high limit event on*/
    boolean levon; /*low limit event on*/
    tfloat sample_rate; /* sample rate in seconds */
    void *onsetdata;
    void *parent;
} threshold_control_struc64;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    char dn[DETECTOR_NAME_LENGTH];
    threshold_control_struc64 thr_cont;
} tctythr64;

// CTY_FIR
#pragma pack(4)
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    word fcnt; /* current number of samples in fir buffer */
    longint foff; /* offset from start of buffer to "f" pointer */
    char fn[FILTER_NAME_LENGTH];
    tfloat fbuffer; /* this is an array */
} tctyfir32;

#pragma pack(8)
typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    word fcnt; /* current number of samples in fir buffer */
    longint foff; /* offset from start of buffer to "f" pointer */
    char fn[FILTER_NAME_LENGTH];
    tfloat fbuffer; /* this is an array */
} tctyfir64;

void gcrcinit(crc_table_type *crctable);
longint gcrccalc(crc_table_type *crctable, byte *p, longint len);

void CopyTctylcq(tctylcq32 *tc32, byte (&out)[8192], crc_table_type &crc_table);
void CopyTctyiir(tctyiir32 *iir32, byte (&out)[8192],
        crc_table_type &crc_table);
void CopyTsystem(tsystem32 *ts32, byte (&out)[8192], crc_table_type &crc_table);
void CopyTctythr(tctythr32 *tc32, byte (&out)[8192], crc_table_type &crc_table);
void CopyTctyfir(tctyfir32 *fir32, byte (&out)[8192],
        crc_table_type &crc_table);

void AddBufferToOutputArray(
        std::vector<std::shared_ptr<unsigned char>> &outputData,
        unsigned char *outbuff)
{
    tctyhdr *phdr;
    unsigned char *puc;
    phdr = (tctyhdr*) outbuff;
    puc = new unsigned char[phdr->size];
    memcpy(puc, outbuff, phdr->size);
    outputData.push_back(std::shared_ptr<unsigned char>(puc));
}
int binq32to64(const char *inputFile, const char *pstanet)
{
    tctyring *pring;
    unsigned char buff[8192];
    unsigned char outbuff[8192];
    char sta[32], net[32];
    std::vector<std::shared_ptr<unsigned char>> outputData;
    unsigned long int rc;

    longint buffsz = 0, total = 0;
    longint crc;
    crc_table_type crc_table;

    tctyhdr *phdr;
    FILE *f = NULL;
    char cPath[PATH_MAX], outFile[PATH_MAX];

    memset(sta, 0, sizeof(sta));
    memset(net, 0, sizeof(net));

    strcpy(cPath, inputFile);

    char *dir = dirname(cPath);

    printf("\n %ld %ld", sizeof(tsystem32), sizeof(tsystem64));
    printf("\n %ld %ld", sizeof(tctyring), sizeof(tctyring64));

    f = fopen(inputFile, "rb");
    if (f == NULL)
    {
        printf("\nError open:%s", inputFile);
        return -1;
    }

    gcrcinit(&crc_table);

    phdr = (tctyhdr*) buff;
    while (feof(f) == 0)
    {
        if (fread(buff, sizeof(buff), 1, f) == 0)
        {
            break;
        }
        buffsz = phdr->size;
        crc = gcrccalc(&crc_table, &buff[sizeof(longint)],
                buffsz - sizeof(longint));

        printf("\nsize:%d id:%d crc: %d, crc calculated:%d", buffsz, phdr->id,
                phdr->crc, crc);

        total += buffsz;

        switch (phdr->id)
        {
        case CTY_THR:
            CopyTctythr((tctythr32*) buff, outbuff, crc_table);
            AddBufferToOutputArray(outputData, outbuff);
            break;

        case CTY_LCQ:
            CopyTctylcq((tctylcq32*) buff, outbuff, crc_table);
            AddBufferToOutputArray(outputData, outbuff);
            break;
        case CTY_IIR:
            CopyTctyiir((tctyiir32*) buff, outbuff, crc_table);
            AddBufferToOutputArray(outputData, outbuff);
            break;
        case CTY_RING:
            pring = (tctyring*) buff;
            memcpy(sta, (void*) &pring->comprec[8], 5);
            memcpy(net, (void*) &pring->comprec[18], 2);
            AddBufferToOutputArray(outputData, buff);
            break;
        case CTY_SYSTEM:
            CopyTsystem((tsystem32*) buff, outbuff, crc_table);
            AddBufferToOutputArray(outputData, outbuff);
            break;
        case CTY_FIR:
            CopyTctyfir((tctyfir32*) buff, outbuff, crc_table);
            AddBufferToOutputArray(outputData, outbuff);
            break;
        default:
            break;
        }

        fseek(f, total, SEEK_SET);
    }

    fclose(f);

    strcpy(sta, trim(sta));
    strcpy(net, trim(net));

    if (*pstanet != 0)
    {
        sprintf(outFile, "%s/q330serv_cont_%s.binq", dir, pstanet);
    }
    else
    {
        if (sta[0] == 0 || net[0] == 0)
        {
            printf("\nStation name is not set. Use sta= argument/n");
            exit(-1);
        }
        else
        {
            sprintf(outFile, "%s/q330serv_cont_%s.%s.binq", dir, sta, net);
        }
    }



    f = fopen(outFile, "wb");

    for (int i = 0; i < outputData.size(); ++i)
    {
        phdr = (tctyhdr*) outputData[i].get();

        rc = fwrite(outputData[i].get(), phdr->size, 1, f);
        if (rc == 0)
        {
            printf("\nError writing file: %s", outFile);
            exit(-1);
        }
    }

    fclose(f);

    f = fopen(outFile, "rb");
    if (f == NULL)
    {
        printf("\nError open:%s", outFile);
        return -1;
    }

    total = 0;
    phdr = (tctyhdr*) buff;
    while (feof(f) == 0)
    {
        if (fread(buff, sizeof(buff), 1, f) == 0)
        {
            break;
        }
        buffsz = phdr->size;
        crc = gcrccalc(&crc_table, &buff[sizeof(longint)],
                buffsz - sizeof(longint));

        printf("\nsize:%d id:%d crc: %d, crc calculated:%d", buffsz, phdr->id,
                phdr->crc, crc);

        total += buffsz;

        fseek(f, total, SEEK_SET);
    }

    fclose(f);
    printf("\nDone\n");
    return 0;
}

// CTY_SYSTEM
void CopyTsystem(tsystem32 *ts32, byte (&out)[8192], crc_table_type &crc_table)
{

    tsystem64 *ts64 = (tsystem64*) &out[0];

    printf("\n %ld %ld", sizeof(tsystem32), sizeof(tsystem64));
    printf("\n");

    ts64->crc = 0; /* CRC of everything following */
    ts64->id = ts32->id; /* what kind of entry */
    ts64->size = sizeof(tsystem64); /* size of this entry */
    ts64->version = ts32->version;
    ts64->high_freq = ts32->high_freq; /* high frequency encoding */
    ts64->serial[0] = ts32->serial[0]; /* serial number of q330 */
    ts64->serial[1] = ts32->serial[1];
    ts64->lasttime = ts32->lasttime; /* data_timetag of last second of data */
    ts64->last_dataqual = ts32->last_dataqual; /* 0-100% */
    ts64->last_dataseq = ts32->last_dataseq; /* data record sequence. sequence continuity important too */
    ts64->comm_event_bitmask = ts32->comm_event_bitmask;
    ts64->reboot_counter = ts32->reboot_counter; /* last reboot counter */

    ts64->crc = gcrccalc(&crc_table, ((byte*) (&out[0]) + sizeof(longint)),
            sizeof(tsystem64) - sizeof(longint));
}

// CTY_LCQ
void CopyTctylcq(tctylcq32 *tc32, byte (&out)[8192], crc_table_type &crc_table)
{
    int i;
    tctylcq64 *tc64 = (tctylcq64*) out;

    printf("\n %ld %ld", sizeof(tctylcq32), sizeof(tctylcq64));
    printf("\n");

    memset(out, 0, sizeof(out));

    tc64->crc = 0; /* CRC of everything following */
    tc64->id = tc32->id; /* what kind of entry */
    tc64->size = sizeof(tctylcq64); /* size of this entry */
    for (i = 0; i < 2; ++i)
        tc64->loc[i] = tc32->loc[i];
    for (i = 0; i < 3; ++i)
        tc64->name[i] = tc32->name[i];
    tc64->lpad = tc32->lpad;
    tc64->lastdtsequence = tc32->lastdtsequence; /* last data record sequence processed */
    tc64->prev_rate = tc32->prev_rate; /* rate when continuity written */
    for (i = 0; i < (MAXMTU - 40); ++i)
        tc64->multbuf.pkt[i] = tc32->multbuf.pkt[i];
    tc64->multbuf.ppkt = 0; // tc32->multbuf.ppkt;
    tc64->prev_delay = tc32->prev_delay; /* delay when continuity written */
    tc64->glast = tc32->glast; /* gen_last_on */
    tc64->con = tc32->con; /* cal_on */
    tc64->cstat = tc32->cstat; /* calstat */
    tc64->overwrite_slipping = tc32->overwrite_slipping;
    tc64->qpad = tc32->qpad;
    tc64->cinc = tc32->cinc; /* calinc */
    tc64->rec_written = tc32->rec_written; /* records written */
    tc64->arec_written = tc32->arec_written; /* archive records written */
    tc64->gnext = tc32->gnext; /* gen_next */
    tc64->last_sample = tc32->last_sample; /* last compression sample */
    tc64->nextrec_tag = tc32->nextrec_tag; /* this is the expected starting time of the next record */
    tc64->lastrec_tag = tc32->lastrec_tag; /* this was the last timetag used */
    tc64->backup_timetag = tc32->backup_timetag;
    tc64->backup_timequal = tc32->backup_timequal;

    tc64->crc = gcrccalc(&crc_table, ((byte*) (&out[0]) + sizeof(longint)),
            sizeof(tctylcq64) - sizeof(longint));
}

void CopyTctyiir(tctyiir32 *iir32, byte (&out)[8192], crc_table_type &crc_table)
{
    int i = 0, j = 0, nOut = 0;
    tctyiir64 *iir64 = (tctyiir64*) &out[0];
    tfloat *src, *dst;

    printf("\n %ld %ld", sizeof(tctyiir32), sizeof(tctyiir64));
    printf("\n");

    nOut = (iir32->size - sizeof(tctyiir32)) / sizeof(tfloat);

    memset(iir64, 0, sizeof(out));

    iir64->crc = 0; /* CRC of everything following */
    iir64->id = iir32->id; /* what kind of entry */
    iir64->size = sizeof(tctyiir64) + (nOut) * sizeof(tfloat); /* size of this entry */
    for (i = 0; i < 2; ++i)
        iir64->loc[i] = iir32->loc[i];
    for (i = 0; i < 2; ++i)
        iir64->name[i] = iir32->name[i];
    iir64->lpad = iir32->lpad;
    for (i = 0; i < FILTER_NAME_LENGTH; ++i)
        iir64->fn[i] = iir32->fn[i];
    for (i = 0; i < MAXSECTIONS + 1; ++i)
    {
        for (j = 0; j < MAXPOLES + 1; ++j)
        {
            iir64->flt[i].x[j] = iir32->flt[i].x[j];
            iir64->flt[i].y[j] = iir32->flt[i].y[j];
        }
    }
    dst = &iir64->outbuf;
    src = &iir32->outbuf;

    for (i = 0; i < nOut + 1; ++i)
        dst[i] = src[i];

    iir64->crc = gcrccalc(&crc_table, ((byte*) (&out[0]) + sizeof(longint)),
            iir64->size - sizeof(longint));
}

void CopyTctythr(tctythr32 *tc32, byte (&out)[8192], crc_table_type &crc_table)
{
    int i;
    tctythr64 *tc64 = (tctythr64*) out;

    printf("\n %ld %ld %ld", sizeof(tctythr32), sizeof(tctythr64), sizeof(out));
    printf("\n");

    memset(out, 0, sizeof(out));

    //tc64->crc /* CRC of everything following */
    tc64->id = tc32->id; /* what kind of entry */
    tc64->size = sizeof(tctythr64); /* size of this entry */
    for (i = 0; i < 2; ++i)
        tc64->loc[i] = tc32->loc[i];
    for (i = 0; i < 3; ++i)
        tc64->name[i] = tc32->name[i];
    tc64->lpad = tc32->lpad;
    for (i = 0; i < DETECTOR_NAME_LENGTH; ++i)
        tc64->dn[i] = tc32->dn[i];
    tc64->thr_cont.default_enabled = tc32->thr_cont.default_enabled;
    tc64->thr_cont.detection_declared = tc32->thr_cont.detection_declared;
    tc64->thr_cont.detector_enabled = tc32->thr_cont.detector_enabled;
    tc64->thr_cont.detector_on = tc32->thr_cont.detector_on;
    tc64->thr_cont.etime = tc32->thr_cont.etime;
    tc64->thr_cont.first_detection = tc32->thr_cont.first_detection;
    tc64->thr_cont.hevon = tc32->thr_cont.hevon;
    tc64->thr_cont.levon = tc32->thr_cont.levon;
    tc64->thr_cont.new_onset = tc32->thr_cont.new_onset;
    tc64->thr_cont.onsetdata = 0;
    tc64->thr_cont.overhi = tc32->thr_cont.overhi;
    tc64->thr_cont.overlo = tc32->thr_cont.overlo;
    tc64->thr_cont.parent = 0;
    tc64->thr_cont.peakhi = tc32->thr_cont.peakhi;
    tc64->thr_cont.peaklo = tc32->thr_cont.peaklo;
    tc64->thr_cont.sampcnt = tc32->thr_cont.sampcnt;
    tc64->thr_cont.sample_rate = tc32->thr_cont.sample_rate;
    tc64->thr_cont.startt = tc32->thr_cont.startt;
    tc64->thr_cont.total_detections = tc32->thr_cont.total_detections;
    tc64->thr_cont.waitdly = tc32->thr_cont.waitdly;

    tc64->crc = gcrccalc(&crc_table, ((byte*) (&out[0]) + sizeof(longint)),
            tc64->size - sizeof(longint));

}

void CopyTctyfir(tctyfir32 *fir32, byte (&out)[8192], crc_table_type &crc_table)
{
    int i = 0, nOut = 0;
    tctyfir64 *fir64 = (tctyfir64*) &out[0];
    tfloat *src, *dst;

    memset(out, 0, sizeof(out));

    printf("\n %ld %ld", sizeof(tctyfir32), sizeof(tctyfir64));
    printf("\n");

    nOut = (fir32->size - sizeof(tctyfir32)) / sizeof(tfloat);

    fir64->id = fir32->id; /* what kind of entry */
    fir64->size = sizeof(tctyfir64) + (nOut) * sizeof(tfloat); /* size of this entry */
    for (i = 0; i < 2; ++i)
        fir64->loc[i] = fir32->loc[i];
    for (i = 0; i < 3; ++i)
        fir64->name[i] = fir32->name[i];
    fir64->lpad = fir32->lpad;
    fir64->fcnt = fir32->fcnt; /* current number of samples in fir buffer */
    fir64->foff = fir32->foff; /* offset from start of buffer to "f" pointer */
    for (i = 0; i < FILTER_NAME_LENGTH; ++i)
        fir64->fn[i] = fir32->fn[i];

    dst = &fir64->fbuffer;
    src = &fir32->fbuffer;

    for (i = 0; i < nOut + 1; ++i)
        dst[i] = src[i];

    fir64->crc = gcrccalc(&crc_table, ((byte*) (&out[0]) + sizeof(longint)),
            fir64->size - sizeof(longint));
}
