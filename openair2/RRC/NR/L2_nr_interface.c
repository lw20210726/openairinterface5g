/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file l2_nr_interface.c
 * \brief layer 2 interface, used to support different RRC sublayer
 * \author Raymond Knopp and Navid Nikaein, WEI-TAI CHEN
 * \date 2010-2014, 2018
 * \version 1.0
 * \company Eurecom, NTUST
 * \email: raymond.knopp@eurecom.fr, kroempa@gmail.com
 */

#include "platform_types.h"
#include "nr_rrc_defs.h"
#include "common/utils/LOG/log.h"
#include "pdcp.h"
#include "common/ran_context.h"
#include "openair2/LAYER2/MAC/mac_vars.h"

#include "intertask_interface.h"

#include "NR_MIB.h"
#include "NR_BCCH-BCH-Message.h"

extern RAN_CONTEXT_t RC;

int8_t mac_rrc_nr_data_req(const module_id_t Mod_idP,
                           const int CC_id,
                           const frame_t frameP,
                           const rb_id_t Srb_id,
                           const uint8_t Nb_tb,
                           uint8_t *const buffer_pP) {

    asn_enc_rval_t enc_rval;
    uint8_t Sdu_size = 0;
    uint8_t sfn_msb = (uint8_t) ((frameP >> 4) & 0x3f);

#ifdef DEBUG_RRC
    LOG_D(RRC, "[eNB %d] mac_rrc_data_req to SRB ID=%ld\n", Mod_idP, Srb_id);
#endif

    gNB_RRC_INST *rrc;
    rrc_gNB_carrier_data_t *carrier;
    NR_BCCH_BCH_Message_t *mib;
    NR_SRB_INFO *srb_info;
    char payload_size, *payload_pP;

    rrc = RC.nrrrc[Mod_idP];
    carrier = &rrc->carrier;
    mib = &carrier->mib;
    srb_info = &carrier->Srb0;

    /* MIBCH */
    if ((Srb_id & RAB_OFFSET) == MIBCH) {


        /// controlResourceSetZero

        switch (*carrier->servingcellconfigcommon->ssbSubcarrierSpacing) {

            case NR_SubcarrierSpacing_kHz15:
                if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz15) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_1_num_indexes;
                } else if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz30) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_2_num_indexes;
                } else {
                    AssertFatal(true,"Invalid subCarrierSpacingCommon\n");
                }
                break;

            case NR_SubcarrierSpacing_kHz30:
                if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz15) {

                    // TODO get min_bw 38.101-1 table 5.3.5-1
                    //if ( (min_bw == 5) || (min_bw == 10) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_3_num_indexes;
                    //} else if (min_bw == 40) {
                    //    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_5_num_indexes;
                    //}

                } else if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz30) {

                    // TODO get min_bw 38.101-1 table 5.3.5-1
                    //if ( (min_bw == 5) || (min_bw == 10) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_4_num_indexes;
                    //} else if (min_bw == 40) {
                    //    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_6_num_indexes;
                    //}

                } else {
                    AssertFatal(true,"Invalid subCarrierSpacingCommon\n");
                }
                break;

            case NR_SubcarrierSpacing_kHz120:
                if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz60) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_7_num_indexes;
                } else if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz120) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_8_num_indexes;
                } else {
                    AssertFatal(true,"Invalid subCarrierSpacingCommon\n");
                }
                break;

            case NR_SubcarrierSpacing_kHz240:
                if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz60) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_9_num_indexes;
                } else if (mib->message.choice.mib->subCarrierSpacingCommon == NR_SubcarrierSpacing_kHz120) {
                    mib->message.choice.mib->pdcch_ConfigSIB1.controlResourceSetZero = rand() % table_38213_13_10_num_indexes;
                } else {
                    AssertFatal(true,"Invalid subCarrierSpacingCommon\n");
                }
                break;

            default:
                AssertFatal(true,"Invalid ssbSubcarrierSpacing\n");
                break;
        }


        /// TODO: searchSpaceZero

        //if(mac->type0_pdcch_ss_mux_pattern == 1 && frequency_range == FR1){
            mib->message.choice.mib->pdcch_ConfigSIB1.searchSpaceZero = rand() % table_38213_13_11_num_indexes;
        /*}
        if(mac->type0_pdcch_ss_mux_pattern == 1 && frequency_range == FR2){
            mib->message.choice.mib->pdcch_ConfigSIB1.searchSpaceZero = rand() % table_38213_13_12_num_indexes;
        }
        if(mac->type0_pdcch_ss_mux_pattern == 2) {
            if((scs_ssb == scs_120kHz) && (scs_pdcch == scs_60kHz)){
                mib->message.choice.mib->pdcch_ConfigSIB1.searchSpaceZero = rand() % table_38213_13_13_num_indexes;
            } else if((scs_ssb == scs_240kHz) && (scs_pdcch == scs_120kHz)){
                mib->message.choice.mib->pdcch_ConfigSIB1.searchSpaceZero = rand() % table_38213_13_14_num_indexes;
            }
        }*/


        //NR_UE_MAC_INST_t *mac = get_mac_inst(Mod_idP);
        //gNB_mac = RC.nrmac[0];
        //mac->type0_pdcch_ss_mux_pattern


        mib->message.choice.mib->systemFrameNumber.buf[0] = sfn_msb << 2;
        enc_rval = uper_encode_to_buffer(&asn_DEF_NR_BCCH_BCH_Message,
                                         NULL,
                                         (void *) mib,
                                         carrier->MIB,
                                         24);
        LOG_D(NR_RRC, "Encoded MIB for frame %d sfn_msb %d (%p), bits %lu\n", frameP, sfn_msb, carrier->MIB,
              enc_rval.encoded);
        buffer_pP[0] = carrier->MIB[0];
        buffer_pP[1] = carrier->MIB[1];
        buffer_pP[2] = carrier->MIB[2];
        LOG_D(NR_RRC, "MIB PDU buffer_pP[0]=%x , buffer_pP[1]=%x, buffer_pP[2]=%x\n", buffer_pP[0], buffer_pP[1],
              buffer_pP[2]);
        AssertFatal (enc_rval.encoded > 0, "ASN1 message encoding failed (%s, %lu)!\n",
                     enc_rval.failed_type->name, enc_rval.encoded);
        return (3);
    }

    /* TODO BCCH SIB1 SIBs */

    /* CCCH */
    if ((Srb_id & RAB_OFFSET) == CCCH) {
        //struct rrc_eNB_ue_context_s *ue_context_p = rrc_eNB_get_ue_context(RC.rrc[Mod_idP],rnti);
        //if (ue_context_p == NULL) return(0);
        //eNB_RRC_UE_t *ue_p = &ue_context_p->ue_context;
        LOG_D(RRC, "[gNB %d] Frame %d CCCH request (Srb_id %ld)\n", Mod_idP, frameP, Srb_id);

        // srb_info=&ue_p->Srb0;

        payload_size = srb_info->Tx_buffer.payload_size;

        // check if data is there for MAC
        if (payload_size > 0) {
            payload_pP = srb_info->Tx_buffer.Payload;
            LOG_D(RRC, "[gNB %d] CCCH (%p) has %d bytes (dest: %p, src %p)\n", Mod_idP, srb_info, payload_size,
                  buffer_pP, payload_pP);
            // Fill buffer
            memcpy((void *) buffer_pP, (void *) payload_pP, payload_size);
            Sdu_size = payload_size;
            srb_info->Tx_buffer.payload_size = 0;
        }
        return Sdu_size;
    }

    return (0);

}