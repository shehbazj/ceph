

#include <sys/stat.h>
#include <iostream>
#include <string>

#include "mds/MDS.h"
#include "osd/OSD.h"
#include "fakeclient/FakeClient.h"

#include "mds/MDCluster.h"
#include "mds/MDCache.h"
#include "mds/MDStore.h"

#include "msg/FakeMessenger.h"

#include "messages/MPing.h"

using namespace std;

__uint64_t ino = 1;



#include "include/config.h"
#define NUMMDS g_conf.num_mds
#define NUMOSD g_conf.num_osd
#define NUMCLIENT g_conf.num_fakeclient

// this parses find output
int play();

int main(int argc, char **argv) {
  cerr << "hi there" << endl;
  
  if (argc > 1) {
	int d = atoi(argv[1]);
	if (d > 0)
	  g_conf.debug = d;
	cerr << " debug level " << d << endl;
  }

  MDCluster *mdc = new MDCluster(NUMMDS, NUMOSD);
  
  // local config settings

  // create mds
  MDS *mds[NUMMDS];
  for (int i=0; i<NUMMDS; i++) {
	mds[i] = new MDS(mdc, i, new FakeMessenger(MSG_ADDR_MDS(i)));
	mds[i]->init();
  }
  
  // create osds
  OSD *osd[NUMOSD];
  for (int i=0; i<NUMOSD; i++) {
	osd[i] = new OSD(i, new FakeMessenger(MSG_ADDR_OSD(i)));
	osd[i]->init();
  }
  
  // create clients
  FakeClient *client[NUMCLIENT];
  for (int i=0; i<NUMCLIENT; i++) {
	client[i] = new FakeClient(mdc, i, new FakeMessenger(MSG_ADDR_CLIENT(i)), g_conf.fakeclient_requests);
	client[i]->init();
  }
  
  // seed initial requests
  for (int i=0; i<NUMCLIENT; i++) 
	//for (int i=0; i<1; i++) 
	client[i]->issue_request();

  // loop
  fakemessenger_do_loop();

  //mds[0]->shutdown_start();
  //fakemessenger_do_loop();

  // 
  if (argc > 1 && 
	  strcmp(argv[1], "nocheck") == 0) {
	cerr << "---- nocheck" << endl;
  } else {
	cout << "---- check ----" << endl;
	for (int i=0; i<NUMMDS; i++) 
	  mds[i]->mdcache->shutdown_pass();
  }
  
  // cleanup
  cout << "cleanup" << endl;
  for (int i=0; i<NUMMDS; i++) {
	if (mds[i]->shutdown_final() == 0) {
	  //cout << "clean shutdown of mds " << i << endl;
	  delete mds[i];
	} else {
	  cout << "problems shutting down mds " << i << endl;
	}
  }
  for (int i=0; i<NUMOSD; i++) {
	if (osd[i]->shutdown() == 0) { 
	  //cout << "clean shutdown of osd " << i << endl;
	  delete osd[i];
	} else {
	  cout << "problems shutting down osd " << i << endl;
	}
  }
  for (int i=0; i<NUMCLIENT; i++) {
	if (client[i]->shutdown() == 0) { 
	  //cout << "clean shutdown of client " << i << endl;
	  delete client[i];
	} else {
	  cout << "problems shutting down client " << i << endl;
	}
  }
  delete mdc;
  cout << "done." << endl;
  return 0;
}

