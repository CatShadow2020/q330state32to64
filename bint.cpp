/*
 * bint.cpp
 *
 *  Created on: Nov 29, 2020
 *      Author: aakimov
 */
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <string>
#include <vector>
#include <linux/limits.h>
#include <libgen.h>
#include "util.h"
#pragma pack(4)

#define BOOM_COUNT 6
#define SENS_CHANNELS 6

enum tgps_stat
{
    GPS_OFF, /* GPS is off */
    GPS_OFF_LOCK, /* Off due to GPS Lock */
    GPS_OFF_PLL, /* Off due to PLL Lock */
    GPS_OFF_LIMIT, /* Off due to Time Limit */
    GPS_OFF_CMD, /* Off due to Command */
    GPS_ON, /* On */
    GPS_ON_AUTO, /* Powered on automatically */
    GPS_ON_CMD, /* Powered on by command */
    GPS_COLDSTART
};
/* In cold start */
enum tgps_fix
{
    GPF_LF, /* GPS Off, never locked */
    GPF_OFF, /* GPS Off, unknown lock */
    GPF_1DF, /* GPS Off, last fix was 1D */
    GPF_2DF, /* GPS Off, last fix was 2D */
    GPF_3DF, /* GPS Off, last fix was 3D */
    GPF_NL, /* GPS On, never locked */
    GPF_ON, /* GPS On, unknown lock */
    GPF_1D, /* 1D Fix */
    GPF_2D, /* 2D Fix */
    GPF_3D, /* 3D Fix */
    GPF_NB
};
/* No GPS board */
enum tpll_stat
{
    PLS_OFF, /* PLL Off */
    PLS_HOLD, /* PLL Hold */
    PLS_TRACK, /* PLL Tracking */
    PLS_LOCK
};
/* PLL Locked */

enum tlogfld
{
    LOGF_MSGS, /* Messages */
    LOGF_CFG, /* Configuration */
    LOGF_TIME, /* Timing Blockettes */
    LOGF_DATA_GAP, /* Data Gaps */
    LOGF_REBOOTS, /* Re-Boots */
    LOGF_RECVBPS, /* Received Bps */
    LOGF_SENTBPS, /* Sent Bps */
    LOGF_COMMATMP, /* Communication Attepts */
    LOGF_COMMSUCC, /* Communication Successes */
    LOGF_PACKRECV, /* Packets Received */
    LOGF_COMMEFF, /* Communications Efficiency */
    LOGF_POCSRECV, /* POC's Received */
    LOGF_IPCHANGE, /* IP Address Changes */
    LOGF_COMMDUTY, /* Communications Duty Cycle */
    LOGF_THRPUT, /* Throughput */
    LOGF_MISSDATA, /* Missing Data */
    LOGF_SEQERR, /* Sequence Errors */
    LOGF_CHKERR, /* Checksum Errors */
    LOGF_NDATLAT, /* Normal Data Latency */
    LOGF_LDATLAT, /* Low Latency Data Latency */
    LOGF_STATLAT, /* Status Latency */
    LOGF_SIZE, /* To get number of fields */
};

enum taccdur
{
    AD_MINUTE, AD_HOUR, AD_DAY
};

#define AC_LAST LOGF_CHKERR

typedef longint taccstats[AC_LAST + 1][AD_DAY + 1];

typedef struct
{ /* for building one dp statistic */
    longint accum;
    longint accum_ds; /* for datastream use */
    int32_t ds_lcq;
    taccminutes minutes;
    longint hours[24];
} taccmstat;

typedef struct
{
    word low_seq; /* last packet number acked */
    word latest; /* latest packet received */
    longword validmap[8];
} tslidestat;

typedef taccmstat taccmstats[AC_LAST + 1];

typedef struct
{ /* operation status */
    string9 station_name; /* network and station */
    word station_port; /* data port number */
    longword station_tag; /* tagid */
    t64 station_serial; /* q330 serial number */
    longword station_reboot; /* time of last reboot */
    longint timezone_offset; /* seconds to adjust computer's clock */
    taccstats accstats; /* accumulated statistics */
    word minutes_of_stats; /* how many minutes of data available to make hour */
    word hours_of_stats; /* how many hours of data available to make day */
    word auxinp; /* bitmap of Aux. inputs */
    longint data_latency; /* data latency (calculated based on host clock) in seconds or INVALID_LATENCY */
    longint status_latency; /* seconds since received status from 330 or INVALID_LATENCY */
    longint runtime; /* running time in seconds since current connection (+) or time it has been down (-) */
    longword totalgaps; /* total number of data gaps since context created */
    single pkt_full; /* percent of Q330 packet buffer full */
    word clock_qual; /* Percent clock quality */
    longint clock_drift; /* Clock drift from GPS in microseconds */
    integer mass_pos[6]; /* mass positions */
    integer calibration_errors; /* calibration error bitmap */
    integer sys_temp; /* Q330 temperature in degrees C */
    single pwr_volt; /* Q330 power supply voltage in volts */
    single pwr_cur; /* Q330 power supply current in amps */
    longint gps_age; /* age in seconds of last GPS clock update, -1 for never updated */
    enum tgps_stat gps_stat; /* GPS Status */
    enum tgps_fix gps_fix; /* GPS Fix */
    enum tpll_stat pll_stat; /* PLL Status */
    double gps_lat; /* Latitude */
    double gps_long; /* Longitude */
    double gps_elev; /* Elevation */
    tslidestat slidecopy; /* sliding window status */
    longword last_data_time; /* Latest data received, 0 for none */
    longword current_ip; /* current IP Address of Q330 */
    word current_port; /* current Q330 UDP Port */
} topstat;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    byte version;
    tseed_net net;
    tseed_stn stn; /* these make sure nothing tricky has happened */
    boolean auto_adjust; /* restore timezone adjustment from continuity file */
    longint timezone_adjust; /* current timezone offset */
    integer mem_required; /* amount of memory required */
    integer thrmem_required; /* amount of thread memory required */
    longint time_written; /* seconds since 2000 that this was written */
    integer stat_minutes; /* minutes worth of info will go in this slot */
    integer stat_hours; /* hours worth will go into this slot */
    longint total_minutes; /* total minutes accumulated */
    double timetag_save; /* for the purposes of calculating data latency */
    double last_status_save; /* for the purposes of calculating status latency */
    longint tag_save; /* tagid save */
    t64 sn_save; /* serial number */
    longword reboot_save; /* reboot time save */
    topstat opstat; /* snapshot of operational status */
    taccmstats accmstats; /* snapshot of opstat generation information */
} tstatic;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    byte dp_src; /* selects among the DP Statistics */
    longword lcq_options;
    integer frame_limit;
    single gap_thresh;
#ifndef OMIT_SEED
    longint rec_written; /* records written */
    longint arec_written; /* archive records written */
    longint last_sample; /* last compression sample */
    double nextrec_tag; /* this is the expected starting time of the next record */
    double lastrec_tag; /* this was the last timetag used */
#endif
} tctydplcq;

//            ------------------- 64 -------------------------------------
#pragma pack(8)
typedef struct
{ /* for building one dp statistic */
    longint accum;
    longint accum_ds; /* for datastream use */
    pointer ds_lcq; /* lcq associated with this statistic, if any */
    taccminutes minutes;
    longint hours[24];
} taccmstat64;

typedef struct
{
    word low_seq; /* last packet number acked */
    word latest; /* latest packet received */
    longword validmap[8];
} tslidestat64;

typedef taccmstat64 taccmstats64[AC_LAST + 1];

typedef struct
{ /* operation status */
    string9 station_name; /* network and station */
    word station_port; /* data port number */
    longword station_tag; /* tagid */
    t64 station_serial; /* q330 serial number */
    longword station_reboot; /* time of last reboot */
    longint timezone_offset; /* seconds to adjust computer's clock */
    taccstats accstats; /* accumulated statistics */
    word minutes_of_stats; /* how many minutes of data available to make hour */
    word hours_of_stats; /* how many hours of data available to make day */
    word auxinp; /* bitmap of Aux. inputs */
    longint data_latency; /* data latency (calculated based on host clock) in seconds or INVALID_LATENCY */
    longint status_latency; /* seconds since received status from 330 or INVALID_LATENCY */
    longint runtime; /* running time in seconds since current connection (+) or time it has been down (-) */
    longword totalgaps; /* total number of data gaps since context created */
    single pkt_full; /* percent of Q330 packet buffer full */
    word clock_qual; /* Percent clock quality */
    longint clock_drift; /* Clock drift from GPS in microseconds */
    integer mass_pos[6]; /* mass positions */
    integer calibration_errors; /* calibration error bitmap */
    integer sys_temp; /* Q330 temperature in degrees C */
    single pwr_volt; /* Q330 power supply voltage in volts */
    single pwr_cur; /* Q330 power supply current in amps */
    longint gps_age; /* age in seconds of last GPS clock update, -1 for never updated */
    enum tgps_stat gps_stat; /* GPS Status */
    enum tgps_fix gps_fix; /* GPS Fix */
    enum tpll_stat pll_stat; /* PLL Status */
    double gps_lat; /* Latitude */
    double gps_long; /* Longitude */
    double gps_elev; /* Elevation */
    tslidestat64 slidecopy; /* sliding window status */
    longword last_data_time; /* Latest data received, 0 for none */
    longword current_ip; /* current IP Address of Q330 */
    word current_port; /* current Q330 UDP Port */
} topstat64;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    byte version;
    tseed_net net;
    tseed_stn stn; /* these make sure nothing tricky has happened */
    boolean auto_adjust; /* restore timezone adjustment from continuity file */
    longint timezone_adjust; /* current timezone offset */
    integer mem_required; /* amount of memory required */
    integer thrmem_required; /* amount of thread memory required */
    longint time_written; /* seconds since 2000 that this was written */
    integer stat_minutes; /* minutes worth of info will go in this slot */
    integer stat_hours; /* hours worth will go into this slot */
    longint total_minutes; /* total minutes accumulated */
    double timetag_save; /* for the purposes of calculating data latency */
    double last_status_save; /* for the purposes of calculating status latency */
    longint tag_save; /* tagid save */
    t64 sn_save; /* serial number */
    longword reboot_save; /* reboot time save */
    topstat64 opstat; /* snapshot of operational status */
    taccmstats64 accmstats; /* snapshot of opstat generation information */
} tstatic64;

typedef struct
{
    longint crc; /* CRC of everything following */
    word id; /* what kind of entry */
    word size; /* size of this entry */
    tlocation loc;
    tseed_name name;
    byte lpad;
    byte dp_src; /* selects among the DP Statistics */
    longword lcq_options;
    integer frame_limit;
    single gap_thresh;
#ifndef OMIT_SEED
    longint rec_written; /* records written */
    longint arec_written; /* archive records written */
    longint last_sample; /* last compression sample */
    double nextrec_tag; /* this is the expected starting time of the next record */
    double lastrec_tag; /* this was the last timetag used */
#endif
} tctydplcq64;

static void CopyTctydplcq(tctydplcq &dldest, tctydplcq64 &dldest64,
        crc_table_type &crc_table)
{
    longint crc = 0;
    int i = 0;

    dldest64.id = dldest.id; /* what kind of entry */
    dldest64.size = sizeof(dldest64); /* size of this entry */
    for (i = 0; i < 2; ++i)
        dldest64.loc[i] = dldest.loc[i];
    for (i = 0; i < 3; ++i)
        dldest64.name[i] = dldest.name[i];
    dldest64.lpad = dldest.lpad;
    dldest64.dp_src = dldest.dp_src; /* selects among the DP Statistics */
    dldest64.lcq_options = dldest.lcq_options;
    dldest64.frame_limit = dldest.frame_limit;
    dldest64.gap_thresh = dldest.gap_thresh;
    dldest64.rec_written = dldest.rec_written; /* records written */
    dldest64.arec_written = dldest.arec_written; /* archive records written */
    dldest64.last_sample = dldest.last_sample; /* last compression sample */
    dldest64.nextrec_tag = dldest.nextrec_tag; /* this is the expected starting time of the next record */
    dldest64.lastrec_tag = dldest.lastrec_tag; /* this was the last timetag used */
    crc = gcrccalc(&crc_table, ((byte*) (&dldest64) + sizeof(longint)),
            dldest64.size - 4);
    dldest64.crc = crc;
}
static void CopyTstatic(tstatic &ts, tstatic64 &ts64, crc_table_type &crc_table)
{
    longint crc = 0;
    int i = 0, j = 0;
    memset(&ts64, 0, sizeof(tstatic));
    ts64.crc = 0; /* CRC of everything following */
    ts64.id = ts.id; /* what kind of entry */
    ts64.size = sizeof(ts64); /* size of this entry */
    ts64.version = ts.version;
    memcpy(&ts64.net, &ts.net, sizeof(ts.net)); //   tseed_net net ;
    memcpy(&ts64.stn, &ts.stn, sizeof(ts.stn)); /* these make sure nothing tricky has happened */
    ts64.auto_adjust = ts.auto_adjust; /* restore timezone adjustment from continuity file */
    ts64.timezone_adjust = ts.timezone_adjust; /* current timezone offset */
    ts64.mem_required = ts.mem_required; /* amount of memory required */
    ts64.thrmem_required = ts.thrmem_required; /* amount of thread memory required */
    ts64.time_written = ts.time_written; /* seconds since 2000 that this was written */
    ts64.stat_minutes = ts.stat_minutes; /* minutes worth of info will go in this slot */
    ts64.stat_hours = ts.stat_hours; /* hours worth will go into this slot */
    ts64.total_minutes = ts.total_minutes; /* total minutes accumulated */
    ts64.timetag_save = ts.timetag_save; /* for the purposes of calculating data latency */
    ts64.last_status_save = ts.last_status_save; /* for the purposes of calculating status latency */
    ts64.tag_save = ts.tag_save; /* tagid save */
    ts64.sn_save[0] = ts.sn_save[0]; /* serial number */
    ts64.sn_save[1] = ts.sn_save[1];
    ts64.reboot_save = ts.reboot_save; /* reboot time save */
    //topstat64 opstat ; /* snapshot of operational status */

    memcpy(ts64.opstat.station_name, ts.opstat.station_name, sizeof(string9)); /* network and station */
    ts64.opstat.station_port = ts.opstat.station_port; /* data port number */
    ts64.opstat.station_tag = ts.opstat.station_tag; /* tagid */
    ts64.opstat.station_serial[0] = ts.opstat.station_serial[0]; /* q330 serial number */
    ts64.opstat.station_serial[1] = ts.opstat.station_serial[1];
    ts64.opstat.station_reboot = ts.opstat.station_reboot; /* time of last reboot */
    ts64.opstat.timezone_offset = ts.opstat.timezone_offset; /* seconds to adjust computer's clock */
    memcpy(ts64.opstat.accstats, ts.opstat.accstats,
            sizeof(ts.opstat.accstats)); /* accumulated statistics */
    ts64.opstat.minutes_of_stats = ts.opstat.minutes_of_stats; /* how many minutes of data available to make hour */
    ts64.opstat.hours_of_stats = ts.opstat.hours_of_stats; /* how many hours of data available to make day */
    ts64.opstat.auxinp = ts.opstat.auxinp; /* bitmap of Aux. inputs */
    ts64.opstat.data_latency = ts.opstat.data_latency; /* data latency (calculated based on host clock) in seconds or INVALID_LATENCY */
    ts64.opstat.status_latency = ts.opstat.status_latency; /* seconds since received status from 330 or INVALID_LATENCY */
    ts64.opstat.runtime = ts.opstat.runtime; /* running time in seconds since current connection (+) or time it has been down (-) */
    ts64.opstat.totalgaps = ts.opstat.totalgaps; /* total number of data gaps since context created */
    ts64.opstat.pkt_full = ts.opstat.pkt_full; /* percent of Q330 packet buffer full */
    ts64.opstat.clock_qual = ts.opstat.clock_qual; /* Percent clock quality */
    ts64.opstat.clock_drift = ts.opstat.clock_drift; /* Clock drift from GPS in microseconds */
    for (i = 0; i < 6; ++i)
        ts64.opstat.mass_pos[i] = ts.opstat.mass_pos[i]; /* mass positions */
    ts64.opstat.calibration_errors = ts.opstat.calibration_errors; /* calibration error bitmap */
    ts64.opstat.sys_temp = ts.opstat.sys_temp; /* Q330 temperature in degrees C */
    ts64.opstat.pwr_volt = ts.opstat.pwr_volt; /* Q330 power supply voltage in volts */
    ts64.opstat.pwr_cur = ts.opstat.pwr_cur; /* Q330 power supply current in amps */
    ts64.opstat.gps_age = ts.opstat.gps_age; /* age in seconds of last GPS clock update, -1 for never updated */
    ts64.opstat.gps_stat = ts.opstat.gps_stat; /* GPS Status */
    ts64.opstat.gps_fix = ts.opstat.gps_fix; /* GPS Fix */
    ts64.opstat.pll_stat = ts.opstat.pll_stat; /* PLL Status */
    ts64.opstat.gps_lat = ts.opstat.gps_lat; /* Latitude */
    ts64.opstat.gps_long = ts.opstat.gps_long; /* Longitude */
    ts64.opstat.gps_elev = ts.opstat.gps_elev; /* Elevation */
    ts64.opstat.slidecopy.latest = ts.opstat.slidecopy.latest; /* sliding window status */
    ts64.opstat.slidecopy.low_seq = ts.opstat.slidecopy.low_seq;
    for (i = 0; i < 8; ++i)
        ts64.opstat.slidecopy.validmap[i] = ts.opstat.slidecopy.validmap[i];

    ts64.opstat.last_data_time = ts.opstat.last_data_time; /* Latest data received, 0 for none */
    ts64.opstat.current_ip = ts.opstat.current_ip; /* current IP Address of Q330 */
    ts64.opstat.current_port = ts.opstat.current_port;

    for (i = 0; i < AC_LAST + 1; ++i)
    {
        ts64.accmstats[i].accum = ts.accmstats[i].accum; /* snapshot of opstat generation information */
        ts64.accmstats[i].accum_ds = ts.accmstats[i].accum_ds;
        // ts64.accmstats[i].ds_lcq = ts.accmstats[i].ds_lcq;
        for (j = 0; j < 24; ++j)
            ts64.accmstats[i].hours[j] = ts.accmstats[i].hours[j];
        for (j = 0; j < 60; ++j)
            ts64.accmstats[i].minutes[j] = ts.accmstats[i].minutes[j];
    }
    crc = gcrccalc(&crc_table, ((byte*) (&ts64) + sizeof(longint)),
            sizeof(ts64) - sizeof(longint));
    ts64.crc = crc;
}

int bint32to64(const char *inputFile, const char *pstanet)
{
    tstatic ts;
    tctydplcq dldest;

    tstatic64 ts64, ts64_1;
    tctydplcq64 dldest64, dldest64_1;

    crc_table_type crc_table;
    longint crc;
    size_t rc;

    char sta[32], net[32];

    FILE *f = NULL;

    gcrcinit(&crc_table);

    char cPath[PATH_MAX], fullPath[PATH_MAX], outFile[PATH_MAX];
    strcpy(cPath, inputFile);
    strcpy(fullPath, inputFile);

    char *dir = dirname(cPath);

    f = fopen(fullPath, "rb");
    if (f == NULL)
    {
        printf("\nError open:%s", fullPath);
        exit(-1);
    }

    fread(&ts, sizeof(ts), 1, f);

    crc = gcrccalc(&crc_table, ((byte*) (&ts) + sizeof(longint)),
            sizeof(ts) - sizeof(longint));
    printf("\ncrc: %d, crc calculated:%d", ts.crc, crc);

    if (ts.crc != crc)
    {
        printf("\nCRC doesn't match");
        exit(-1);
    }

    fseek(f, ts.size, SEEK_SET);

    fread(&dldest, sizeof(tctydplcq), 1, f);
    crc = gcrccalc(&crc_table, ((byte*) (&dldest) + sizeof(longint)),
            dldest.size - 4);
    printf("\ncrc: %d, crc calculated:%d", dldest.crc, crc);
    fclose(f);

    if (dldest.crc != crc)
    {
        printf("\nCRC doesn't match");
        exit(-1);
    }

    CopyTstatic(ts, ts64, crc_table);
    CopyTctydplcq(dldest, dldest64, crc_table);

    memset(sta, 0, sizeof(sta));
    memset(net, 0, sizeof(net));
    memcpy(net, ts64.net, sizeof(ts64.net));
    memcpy(sta, ts64.stn, sizeof(ts64.stn));
    strcpy(sta, trim(sta));
    strcpy(net, trim(net));

    if (*pstanet != 0)
    {
        sprintf(outFile, "%s/q330serv_cont_%s.bint", dir, pstanet);
    }
    else
    {
        if (sta[0] == 0 || net[0] == 0)
        {
            printf("\nStation name is not set. Use sta= argument\n");
            exit(-1);
        }
        else
        {
            sprintf(outFile, "%s/q330serv_cont_%s.%s.bint", dir, sta, net);
        }
    }

    f = fopen(outFile, "wb");
    if (f == NULL)
    {
        printf("\nError open file: %s", outFile);
        exit(-1);
    }

    rc = fwrite(&ts64, sizeof(ts64), 1, f);
    if (rc == 0)
    {
        printf("\nError writing file: %s", outFile);
        exit(-1);
    }
    rc = fwrite(&dldest64, sizeof(dldest64), 1, f);
    if (rc == 0)
    {
        printf("\nError writing file: %s", outFile);
        exit(-1);
    }
    fclose(f);

// ---------------------------------------------------------------------------------
    f = fopen(outFile, "rb");
    if (f == NULL)
    {
        printf("\nError open file: %s", outFile);
        exit(-1);
    }

    rc = fread(&ts64_1, sizeof(ts64_1), 1, f);
    if (rc == 0)
    {
        printf("\nError reading file: %s", outFile);
        exit(-1);
    }

    crc = gcrccalc(&crc_table, ((byte*) (&ts64_1) + sizeof(longint)),
            sizeof(ts64_1) - sizeof(longint));
    printf("\ncrc: %d, crc calculated:%d", ts64_1.crc, crc);

    if (ts64_1.crc != crc)
    {
        printf("\nCRC doesn't match");
        exit(-1);
    }

    rc = fread(&dldest64_1, sizeof(tctydplcq64), 1, f);
    if (rc == 0)
    {
        printf("\nError reading file: %s", outFile);
        exit(-1);
    }

    crc = gcrccalc(&crc_table, ((byte*) (&dldest64_1) + sizeof(longint)),
            dldest64_1.size - 4);
    printf("\ncrc: %d, crc calculated:%d", dldest64_1.crc, crc);

    if (dldest64_1.crc != crc)
    {
        printf("\nCRC doesn't match");
        exit(-1);
    }

    fclose(f);

    printf("\nDone\n");

    return 0;

}

