// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2011 New Dream Network
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License version 2.1, as published by the Free Software
 * Foundation.	See file COPYING.
 *
 */

#ifndef __LIBRBD_HPP
#define __LIBRBD_HPP

#include <stdbool.h>
#include <string>
#include <list>
#include <map>
#include <vector>
#include "buffer.h"
#include "librados.hpp"
#include "librbd.h"

namespace librbd {
  class RBDClient;
  typedef void *pool_t;
  typedef void *image_t;
#if 0 // for IO
  typedef void *completion_t;
  typedef void (*callback_t)(completion_t cb, void *arg);
#endif

  typedef struct {
    uint64_t id;
    uint64_t size;
    std::string name;
  } snap_info_t;

  typedef struct {
    uint64_t size;
    uint64_t obj_size;
    uint64_t num_objs;
    int order;
  } image_info_t;

class RBD
{
  RBDClient *client;

public:
  RBD() {}
  ~RBD() {}

  /* We don't allow assignment or copying */
  RBD(const RBD& rhs);
  const RBD& operator=(const RBD& rhs);

  int initialize(int argc, const char *argv[]);
  void shutdown();

  void version(int *major, int *minor, int *extra);

  int open_pool(const char *pool_name, pool_t *pool);
  int close_pool(pool_t pool);
  int open_image(pool_t pool, const char *name, image_t *image, const char *snap_name);
  int close_image(image_t image);
  int create(pool_t pool, const char *name, size_t size);
  int remove(pool_t pool, const char *name);
  int resize(pool_t pool, image_t image, size_t size);
  int stat(pool_t pool, image_t image, image_info_t& info);
  int list(pool_t pool, std::vector<string>& names);
  int copy(pool_t src_pool, const char *srcname, pool_t dest_pool, const char *destname);
  int rename(pool_t src_pool, const char *srcname, const char *destname);

  int create_snap(pool_t pool, image_t image, const char *snap_name);
  int remove_snap(pool_t pool, image_t image, const char *snap_name);
  int rollback_snap(pool_t pool, image_t image, const char *snap_name);
  int list_snaps(pool_t pool, image_t image, std::vector<snap_info_t>& snaps);
  int set_snap(pool_t pool, image_t image, const char *snap_name);

  void get_rados_pools(pool_t pool, librados::pool_t *md_pool, librados::pool_t *data_pool);
};

}

#endif
