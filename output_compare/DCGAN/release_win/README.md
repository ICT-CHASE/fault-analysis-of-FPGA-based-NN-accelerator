# Result handler for DCGAN

output processing: generate picture from output file.  
result compare: compare pictures to calculate SSIM and mean SSIM.

## File path

| Name | Property | Explanation |
|:-- |:-- |:-- |
| output/ | Required | npu output to be processed read from SD card |
| error_list.txt | Required | list of system exceptions |
| pic_original/ | Required | Error-free pictures for comparison, get by runs without error |
| pic/ | Generated | path for generated pictures |
| compare/ | Generated | path for compare result |

## How to use



    run.bat

and input the number of pictures to be process

## Compare result

### mssim.txt

Mean SSIM of comparison of all pictures.

### result_total.txt

Comparison classification count result

### compare_result.txt

Comparison result of all pictures

| Column | Explanation |
| --:|:-- |
| 1 | number of run |
| 2 | Comparison result classification |
| 3 | SSIM |

---------------------------------
