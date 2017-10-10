/*
 * H.265 video codec.
 * Copyright (c) 2013-2014 struktur AG, Dirk Farin <farin@struktur.de>
 *
 * Authors: Dirk Farin <farin@struktur.de>
 *
 * This file is part of libde265.
 *
 * libde265 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libde265 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libde265.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCREENSHARING_CORE_H
#define SCREENSHARING_CORE_H

#include "libde265/nal-parser.h"
#include "libde265/decctx.h"
#include "libde265/encoder/encoder-types.h"
#include "libde265/slice.h"
#include "libde265/scan.h"
#include "libde265/intrapred.h"
#include "libde265/transform.h"
#include "libde265/fallback-dct.h"
#include "libde265/quality.h"
#include "libde265/fallback.h"
#include "libde265/configparam.h"

#include "libde265/encoder/encoder-core.h"


class EncoderCore_Screensharing : public EncoderCore
{
 public:

  struct encoder_params
  {
    encoder_params();

    void registerParams(config_parameters& config);


    // CB quad-tree

    option_int min_cb_size;
    option_int max_cb_size;

    option_int min_tb_size;
    option_int max_tb_size;

    option_int max_transform_hierarchy_depth_intra;
    option_int max_transform_hierarchy_depth_inter;


    option_SOP_Structure sop_structure;

    sop_creator_trivial_low_delay::params mSOP_LowDelay;


    // --- Algo_TB_IntraPredMode

    option_ALGO_TB_IntraPredMode        mAlgo_TB_IntraPredMode;
    option_ALGO_TB_IntraPredMode_Subset mAlgo_TB_IntraPredMode_Subset;

    //Algo_TB_IntraPredMode_FastBrute::params TB_IntraPredMode_FastBrute;
    //Algo_TB_IntraPredMode_MinResidual::params TB_IntraPredMode_MinResidual;


    // --- Algo_TB_Split_BruteForce

    //Algo_TB_Split_BruteForce::params  TB_Split_BruteForce;


    // --- Algo_CB_IntraPartMode

    option_ALGO_CB_IntraPartMode mAlgo_CB_IntraPartMode;

    option_ALGO_CB_Skip mAlgo_CB_Skip;


    // --- Algo_CB_Split

    // --- Algo_CTB_QScale

    //Algo_CTB_QScale_Constant::params    CTB_QScale_Constant;

    option_MEMode mAlgo_MEMode;


    // intra-prediction

    enum IntraPredSearch intraPredSearch;


    // rate-control

    enum RateControlMethod rateControlMethod;
    option_ALGO_TB_RateEstimation mAlgo_TB_RateEstimation;

    //int constant_QP;
    //int lambda;
  };


  void registerParams(config_parameters& config) override {
    mAlgo_CTB_QScale_Constant.registerParams(config);
    mAlgo_CB_IntraPartMode_Fixed.registerParams(config);
    mAlgo_CB_InterPartMode_Fixed.registerParams(config);
    mAlgo_CB_PCM.registerParams(config);
    mAlgo_PB_MV_Test.registerParams(config);
    mAlgo_PB_MV_Search.registerParams(config);
    mAlgo_TB_IntraPredMode_FastBrute.registerParams(config);
    mAlgo_TB_IntraPredMode_MinResidual.registerParams(config);
    mAlgo_TB_Split_BruteForce.registerParams(config);
    mAlgo_CB_Skip_BruteForce.registerParams(config);

    params.registerParams(config);
  }

  // Build algorithm graph and set algorithm module parameters
  void initialize(encoder_picture_buffer*,
                  encoder_context*) override;

  void push_picture(image_ptr img) override;
  void push_end_of_input() override { mSOPCreator->insert_end_of_input(); }


  virtual Algo_CTB* getCTBAlgo() { return &mAlgo_CTB_QScale_Constant; }
  virtual int get_CTB_size_log2() const;

  void fill_headers(std::shared_ptr<video_parameter_set> vps,
                    std::shared_ptr<seq_parameter_set> sps,
                    std::shared_ptr<pic_parameter_set> pps,
                    image_ptr img) const;

  virtual int getPPS_QP() const { return mAlgo_CTB_QScale_Constant.getQP(); }

 private:
  encoder_params params;

  std::shared_ptr<sop_creator> mSOPCreator;

  Algo_CTB_QScale_Constant         mAlgo_CTB_QScale_Constant;

  Algo_CB_Split_BruteForce         mAlgo_CB_Split_BruteForce;
  Algo_CB_Skip_BruteForce          mAlgo_CB_Skip_BruteForce;
  Algo_CB_Skip_ScreenFast          mAlgo_CB_Skip_ScreenFast;
  Algo_CB_MV_Screen                mAlgo_CB_MV_Screen;
  Algo_CB_MV_ScreenRegion          mAlgo_CB_MV_ScreenRegion;
  Algo_CB_IntraInter_BruteForce    mAlgo_CB_IntraInter_BruteForce;

  Algo_CB_IntraPartMode_BruteForce mAlgo_CB_IntraPartMode_BruteForce;
  Algo_CB_IntraPartMode_Fixed      mAlgo_CB_IntraPartMode_Fixed;
  Algo_CB_PCM                      mAlgo_CB_PCM;

  Algo_CB_InterPartMode_Fixed      mAlgo_CB_InterPartMode_Fixed;
  Algo_CB_MergeIndex_Fixed         mAlgo_CB_MergeIndex_Fixed;

  Algo_PB_MV_Test                  mAlgo_PB_MV_Test;
  Algo_PB_MV_Search                mAlgo_PB_MV_Search;

  Algo_TB_Split_BruteForce          mAlgo_TB_Split_BruteForce;

  Algo_TB_IntraPredMode_BruteForce  mAlgo_TB_IntraPredMode_BruteForce;
  Algo_TB_IntraPredMode_FastBrute   mAlgo_TB_IntraPredMode_FastBrute;
  Algo_TB_IntraPredMode_MinResidual mAlgo_TB_IntraPredMode_MinResidual;

  Algo_TB_Transform                 mAlgo_TB_Transform;
  Algo_TB_RateEstimation_None       mAlgo_TB_RateEstimation_None;
  Algo_TB_RateEstimation_Exact      mAlgo_TB_RateEstimation_Exact;
};


#endif
