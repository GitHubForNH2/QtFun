//
//  utils.h
//  YHROV-ARM_UnitTest
//
//  Created by CmST0us on 16/10/29.
//  Copyright © 2016年 广州睿航电子科技有限公司. All rights reserved.
//

#ifndef utils_h
#define utils_h
///////////////////////
//  |Config    Block|

#define PROJECT_DEBUG 1


//  |       End     |
/////////////////////////


#include <stdio.h>

#if PROJECT_DEBUG
#define LOG(x) printf("[DEBUG][%s]: %s\n",__func__, (x))
#define SHOW(x) printf("%s", (x))
#define WARN_LOG(x) printf("[WARN][%s]: %s\n",__func__, (x))
#define ERR_LOG(x)  printf("[ERROR][%s]: %s\n",__func__, (x))
#else

#define LOG(x) ""
#define WARN_LOG(x) ""
#define ERR_LOG(x) ""

#endif


#endif /* utils_h */
