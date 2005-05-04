
#include "include/config.h"

//#define MDS_CACHE_SIZE        4*10000   -> <20mb
//#define MDS_CACHE_SIZE        80000         62mb

#define AVG_PER_INODE_SIZE    400
#define MDS_CACHE_MB_TO_INODES(x) ((x)*1000000/AVG_PER_INODE_SIZE)

#define MDS_CACHE_SIZE       MDS_CACHE_MB_TO_INODES( 100 )
//#define MDS_CACHE_SIZE 25000  // 


md_config_t g_conf = {
  num_mds: 3,
  num_osd: 10,
  num_client: 1,

  osd_cow: false, // crashy? true,  

  client_cache_size: 400,
  client_cache_mid: .5,
  client_use_random_mds:  false,
  
  log_messages: true,
  log_interval: 10.0,
  
  mdlog_max_len: 1000,
  mdlog_max_trimming: 16,
  mdlog_read_inc: 4096,

  fake_clock: true,
  fakemessenger_serialize: true,// false,

  debug: 10,

  mdcache_size: 5000, //MDS_CACHE_SIZE,
  mdcache_mid: .8,

  mdbal_replicate_threshold: 500,
  mdbal_unreplicate_threshold: 200,

  mds_heartbeat_op_interval: 200,
  mds_verify_export_dirauth: true,

  // fakeclient
  num_fakeclient: 10,
  fakeclient_requests: 10,
  fakeclient_deterministic: false,

  fakeclient_op_statfs:     false,

  fakeclient_op_stat:     10,
  fakeclient_op_lstat:      false,
  fakeclient_op_utime:    10,   // untested
  fakeclient_op_chmod:    10,
  fakeclient_op_chown:    10,   // untested

  fakeclient_op_readdir:  10,
  fakeclient_op_mknod:    100,
  fakeclient_op_link:       false,
  fakeclient_op_unlink:   10,
  fakeclient_op_rename:   100,

  fakeclient_op_mkdir:    100,
  fakeclient_op_rmdir:    10,
  fakeclient_op_symlink:  10,

  fakeclient_op_openrd:   10,
  fakeclient_op_openwr:   0,
  fakeclient_op_openwrc:  0,
  fakeclient_op_read:       false,  // osd!
  fakeclient_op_write:      false,  // osd!
  fakeclient_op_truncate:   false,
  fakeclient_op_fsync:      false,
  fakeclient_op_close:    20
};

