#include <stdio.h>
#include <sys/types.h>
#include <Rectangle.h> /* Rectangle ASN.1 type */
int main(int ac, char **av) {
    char buf[1024]; /* Temporary buffer */
    asn_dec_rval_t rval; /* Decoder return value */
    Rectangle_t *rectangle = 0; /* Type to decode. Note this 01! */
    FILE *fp; /* Input file handler */
    size_t size; /* Number of bytes read */
    char *filename; /* Input file name */
    
    /* Section 1.4*/
    int ret; /* Return value */
    char errbuf[128]; /* Buffer for error message */
    size_t errlen = sizeof(errbuf); /* Size of the buffer */
    
    
    /* Require a single filename argument */
    if(ac != 2) {
        fprintf(stderr, "Usage: %s <file.ber>\n", av[0]);
        exit(1);
    } else {
        filename = av[1];
    }
    /* Open input file as readonly binary */
    fp = fopen(filename, "rb");
    if(!fp) {
        perror(filename);
        exit(1);
    }
    /* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    if(!size) {
        fprintf(stderr, "%s: Empty or broken\n", filename);
        exit(1);
    }
    /* Decode the input buffer as Rectangle type */
    rval = ber_decode(0, &asn_DEF_Rectangle, (void **)&rectangle, buf, size);
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken Rectangle encoding at byte %ld\n", filename,(long)rval.consumed);
    }
    /* Print the decoded Rectangle type as XML */
    xer_fprint(stdout, &asn_DEF_Rectangle, rectangle);
    printf("The value received: Height=%d  Width=%d",rectangle->height,rectangle->width);

    /*section 1.4*/
    ret = asn_check_constraints(&asn_DEF_Rectangle, rectangle, errbuf, &errlen);
    /* assert(errlen < sizeof(errbuf)); // you may rely on that */
    if(ret) {
        fprintf(stderr, "Constraint validation failed: %s\n", errbuf);
        exit(1); /* Replace with appropriate action */
    }

    return 0; /* Decoding finished successfully */
}
