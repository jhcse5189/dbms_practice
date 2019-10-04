

    /* create the file of 10 records */
    fp = fopen("junk", "w");
    if (!fp)
      return 1;
    for (i = 1; i <= 11; i++) {
      r.value = i;
      fwrite(&r, sizeof(record), 1, fp);
    }
    fclose(fp);



    /* read the 10 records */
    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    for (i = 1; i <= 11; i++) {
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
    }
    fclose(fp);
    printf("\n");


    /* use fseek to read the 10 records
       in reverse order */

    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    for (i = 10; i >= 0; i--) {
      fseek(fp, sizeof(record) * i, SEEK_SET);
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
    }
    fclose(fp);
    printf("\n");


    /* use fseek to read the 10 record
       in reverse order */

    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < 5; i++) {
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
      fseek(fp, sizeof(record), SEEK_CUR);
    }
    fclose(fp);
    printf("\n");


    /* use fseek to read 4th record,
       change it, and write it back */

    fp = fopen("junk", "r+");
    if (!fp)
      return 1;
    fseek(fp, sizeof(record) * 3, SEEK_SET);
    fread(&r, sizeof(record), 1, fp);
    r.value = 100;

    fseek(fp, sizeof(record) * 3, SEEK_SET);
    fwrite(&r, sizeof(record), 1, fp);
    fclose(fp);
    printf("\n");


    /* read the 10 records to insure
       4th record was changed */

    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    for (i = 1; i <= 10; i++) {
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
    }
    fclose(fp);
    printf("\n");

    return 0;
}
