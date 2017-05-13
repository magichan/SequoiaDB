/*******************************************************************************


   Copyright (C) 2011-2014 SequoiaDB Ltd.

   This program is free software: you can redistribute it and/or modify
   it under the term of the GNU Affero General Public License, version 3,
   as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warrenty of
   MARCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program. If not, see <http://www.gnu.org/license/>.

   Source File Name = pmdkMain.cpp

   Descriptive Name = Process MoDel Main

   When/how to use: this program may be used on binary and text-formatted
   versions of PMD component. This file contains main function for SequoiaDB,
   and all other process-initialization code.

   Dependencies: N/A

   Restrictions: N/A

   Change Activity:
   defect Date        Who Description
   ====== =========== === ==============================================
          09/14/2012  TW  Initial Draft

   Last Changed =

*******************************************************************************/


#include "ossVer.h"
#include "pmd.hpp"
#include "rtn.hpp"
#include "ossProc.hpp"
#include "utilCommon.hpp"
#include "pmdStartup.hpp"
#include "optQgmStrategy.hpp"
#include "pdTrace.hpp"
#include "pmdTrace.hpp"
#include "pmdController.hpp"

using namespace std;
using namespace bson;

namespace engine
{
   /*
    * This function resolve all input arguments from command line
    * It first construct options_description to register all
    * possible arguments we may have
    * And then it will to load from config file
    * Then it will parse command line input again to override config file
    * Basically we want to make sure all parameters that
    * specified in config file
    * can be simply overrided from commandline
    */
   // PD_TRACE_DECLARE_FUNCTION ( SDB_PMDRESVARGS, "pmdResolveArguments" )
   INT32 pmdResolveArguments( INT32 argc, CHAR** argv )
   {
      INT32 rc = SDB_OK ;
      PD_TRACE_ENTRY ( SDB_PMDRESVARGS ) ;
      CHAR exePath[ OSS_MAX_PATHSIZE + 1 ] = { 0 } ;

      rc = ossGetEWD( exePath, OSS_MAX_PATHSIZE ) ;
      if ( rc )
      {
         std::cerr << "Get module path failed: " << rc << std::endl ;
         goto error ;
      }

      rc = pmdGetOptionCB()->init( argc, argv, exePath ) ;
      if ( SDB_PMD_HELP_ONLY == rc || SDB_PMD_VERSION_ONLY == rc )
      {
         PMD_SHUTDOWN_DB( SDB_OK ) ;
         rc = SDB_OK;
         goto done;
      }
      else if ( rc )
      {
         goto error;
      }

   done :
      PD_TRACE_EXITRC ( SDB_PMDRESVARGS, rc );
      return rc ;
   error :
      goto done ;
   }

   void pmdOnQuit()
   {
      PMD_SHUTDOWN_DB( SDB_INTERRUPT ) ;
   }

   static INT32 _pmdSystemInit()
   {
      INT32 rc = SDB_OK ;
      SDB_START_TYPE startType = SDB_START_NORMAL ;
      BOOLEAN bOk = TRUE ;

      rc = pmdGetStartup().init( pmdGetOptionCB()->getDbPath() ) ;
      PD_RC_CHECK( rc, PDERROR, "Start up check failed[rc:%d]", rc ) ;
      // 打开参数对应的文件，尝试某些操作

      startType = pmdGetStartup().getStartType() ;
      bOk = pmdGetStartup().isOK() ;
      PD_LOG( PDEVENT, "Start up from %s, data is %s",
              pmdGetStartTypeStr( startType ),
              bOk ? "normal" : "abnormal" );
      // 日志记录

      rc = getQgmStrategyTable()->init() ;
      PD_RC_CHECK( rc, PDERROR, "Init qgm strategy table failed, rc: %d",
                   rc ) ;

   done:
      return rc ;
   error:
      goto done ;
   }

   static INT32 _pmdPostInit()
   {
      INT32 rc = SDB_OK ;

      if ( SDB_ROLE_STANDALONE == pmdGetDBRole() ||
           SDB_ROLE_OM == pmdGetDBRole() )
      {
         pmdSetPrimary( TRUE ) ;

         if ( !pmdGetStartup().isOK() )
         {
            pmdEDUCB *cb = pmdGetThreadEDUCB() ;
            rc = rtnRebuildDB( cb ) ;
            PD_RC_CHECK( rc, PDERROR, "Failed to rebuild database, rc: %d",
                         rc ) ;

            rc = sdbGetDPSCB()->move( 0, 0 ) ;
            if ( rc )
            {
               PD_LOG( PDERROR, "Move dps to begin failed, rc: %d", rc ) ;
               goto error ;
            }
            PD_LOG( PDEVENT, "Clean dps logs succeed." ) ;
            PD_LOG( PDEVENT, "Rebuild database succeed." ) ;
            pmdGetStartup().ok( TRUE ) ;
         }
      }

   done:
      return rc ;
   error:
      goto done ;
   }

   #define PMD_START_WAIT_TIME         ( 60000 )

   // PD_TRACE_DECLARE_FUNCTION ( SDB_PMDMSTTHRDMAIN, "pmdMasterThreadMain" )
   INT32 pmdMasterThreadMain ( INT32 argc, CHAR** argv )
   {
     // 开启主线程
      INT32      rc       = SDB_OK ;
      PD_TRACE_ENTRY ( SDB_PMDMSTTHRDMAIN );
      pmdKRCB   *krcb     = pmdGetKRCB () ; // 数据库 参数 , 在 pmd.h 中已定义
      UINT32     startTimerCount = 0 ;
      CHAR      verText[ OSS_MAX_PATHSIZE + 1 ] = { 0 } ;

      rc = pmdResolveArguments ( argc, argv ) ; // 解析参数，覆盖 config
      if ( rc )
      {
         ossPrintf( "Failed resolving arguments(error=%d), exit"OSS_NEWLINE,
                    rc ) ;
         goto error ;
      }
      if ( PMD_IS_DB_DOWN ) // 利用 宏展开，判断是否 down 掉
      {
         return rc ;
      }

      sdbEnablePD( pmdGetOptionCB()->getDiagLogPath(),
                   pmdGetOptionCB()->diagFileNum() ) ;
      // 开启。。。
      setPDLevel( (PDLEVEL)( pmdGetOptionCB()->getDiagLevel() ) ) ;
      // 设置日志等级

      ossSprintVersion( "Version", verText, OSS_MAX_PATHSIZE, FALSE ) ;

      PD_LOG ( ( getPDLevel() > PDEVENT ? PDEVENT : getPDLevel() ) ,
               "Start sequoiadb(%s) [%s]...",
               pmdGetOptionCB()->krcbRole(), verText ) ;

      {
         BSONObj confObj ;
         krcb->getOptionCB()->toBSON( confObj ) ;
         PD_LOG( PDEVENT, "All configs: %s", confObj.toString().c_str() ) ;
      }
      // 把配置存入日志

      {
         ossProcLimits limitInfo ;
         rc = limitInfo.init() ;
         // 初始化限制信息
         if ( SDB_OK != rc )
         {
            PD_LOG( PDWARNING, "can not init limit info:%d", rc ) ;
         }
         else
         {
            PD_LOG( PDEVENT, "dump limit info:\n%s", limitInfo.str().c_str() ) ;
            INT64 sort = -1 ;
            INT64 hard = -1 ;
            if ( !limitInfo.getLimit( OSS_LIMIT_VIRTUAL_MEM, sort, hard ) )
            {
               PD_LOG( PDWARNING, "can not get limit of memory space!" ) ;
            }
            else if ( -1 != sort || -1 != hard )
            {
               PD_LOG( PDWARNING, "virtual memory is not unlimited!" ) ;
            }
         }
      }

      rc = pmdEnableSignalEvent( pmdGetOptionCB()->getDiagLogPath(),
                                 (PMD_ON_QUIT_FUNC)pmdOnQuit ) ;
      // pmd 开启单事件信号, 设置获得停止信号时，如何操作
      PD_RC_CHECK ( rc, PDERROR, "Failed to enable trap, rc: %d", rc ) ;
      // 错误日志记录

      sdbGetPMDController()->registerCB( pmdGetDBRole() ) ;
      // sdbGetPMDController 返回一个全局静态 pdmController 变量　
      // pdmController 利用 registerCB 函数根据参数调用 PMD_REGISTER_CB , 依次加载静态变量
      // PMD_REGISTER_CB 调用 _SDB_KRCB 的 registerCB  的方法
      // 把静态变量的参数加入 _SDB_KRCB 的 _arrayCBS 和 _array0args 数组中

      rc = _pmdSystemInit() ;
      // _pmdStartup 类执行 init  
     //这是函数从指定的路径中读取启动文件并初始化，然后初始化SQL相关的策略。
     //启动文件是一个隐藏文件，当数据库正常或者异常退出时候，
     //会记录数据库的状态。如果是从异常退出，则在再次启动的时候，会重新找主节点做全量同步，是自身数据和主节点一致
      
      if ( rc )
      {
         goto error ;
      }

      rc = krcb->init() ;
      // _pmdKRCB::init 函数　
      // 依次启动各个模块

      if ( rc )
      {
         PD_LOG( PDERROR, "Failed to init krcb, rc: %d", rc ) ;
         goto error ;
      }

      rc = _pmdPostInit() ;
      if ( rc )
      {
         goto error ;
      }

      while ( PMD_IS_DB_UP && startTimerCount < PMD_START_WAIT_TIME &&
              !krcb->isBusinessOK() )
        // PMD_IS_DB_UP 启动，启动时间限制， 通过 krcb 设置 _BusinessOK参数
      {
         ossSleepmillis( 100 ) ;
         startTimerCount += 100 ;
      }

      if ( PMD_IS_DB_DOWN )
      {
         rc = krcb->getExitCode() ;
         PD_LOG( PDERROR, "Start failed, rc: %d", rc ) ;
         goto error ;
      }
      else if ( startTimerCount >= PMD_START_WAIT_TIME )
      {
         PD_LOG( PDWARNING, "Start warning (timeout)" ) ;
      }
      // 分析启动错误

      {
         EDUID agentEDU = PMD_INVALID_EDUID ;
         pmdEDUMgr *eduMgr = pmdGetKRCB()->getEDUMgr() ;
         eduMgr->startEDU ( EDU_TYPE_PIPESLISTENER,
                            (void*)pmdGetOptionCB()->getServiceAddr(),
                            &agentEDU ) ;
         // 管道监听者　
         eduMgr->regSystemEDU ( EDU_TYPE_PIPESLISTENER, agentEDU ) ;
         // 把这个EDU 放入 systemEDU 中

         rc = eduMgr->waitUntil( agentEDU, PMD_EDU_RUNNING ) ;
         // 等待 EDU running 
         PD_RC_CHECK( rc, PDERROR, "Wait pipe listener to running "
                      "failed, rc: %d", rc ) ;
      }

#if defined (_LINUX)
      {
         CHAR pmdProcessName [ OSS_RENAME_PROCESS_BUFFER_LEN + 1 ] = {0} ;
         ossSnprintf ( pmdProcessName, OSS_RENAME_PROCESS_BUFFER_LEN,
                       "%s(%s) %s", utilDBTypeStr( pmdGetDBType() ),
                       pmdGetOptionCB()->getServiceAddr(),
                       utilDBRoleShortStr( pmdGetDBRole() ) ) ;
         ossEnableNameChanges ( argc, argv ) ;
         ossRenameProcess ( pmdProcessName ) ;
      }
#endif // _LINUX

      while ( PMD_IS_DB_UP )
      {
         ossSleepsecs ( 1 ) ;
         sdbGetPMDController()->onTimer( OSS_ONE_SEC ) ;
      }
      rc = krcb->getExitCode() ;

   done :
      PMD_SHUTDOWN_DB( rc ) ;
      pmdSetQuit() ;
      krcb->destroy () ;
      rc = krcb->getExitCode() ;
      if ( krcb->needRestart() )
      {
         pmdGetStartup().restart( TRUE, rc ) ;
      }
      pmdGetStartup().final() ;
      PD_LOG ( PDEVENT, "Stop sequoiadb, exit code: %d",
               krcb->getExitCode() ) ;
      PD_TRACE_EXITRC ( SDB_PMDMSTTHRDMAIN, rc );
      return utilRC2ShellRC( rc ) ;
   error :
      goto done ;
   }

}

/**************************************/
/*   DATABASE MAIN FUNCTION           */
/**************************************/
//PD_TRACE_DECLARE_FUNCTION ( SDB_PMDMAIN, "main" )
INT32 main ( INT32 argc, CHAR** argv )
{
   INT32 rc = SDB_OK ;
   PD_TRACE_ENTRY ( SDB_PMDMAIN );
   rc = engine::pmdMasterThreadMain ( argc, argv ) ;
   PD_TRACE_EXITRC ( SDB_PMDMAIN, rc );
   return rc ;
}

