/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/
#include "gtest/gtest.h"
#include "ocprovisioning_manager.h"   //GAR case-sensitivity

static OicSecAcl_t acl1;
static OicSecAcl_t acl2;
static OCProvisionDev_t pDev1;
static OCProvisionDev_t pDev2;
static OicSecCredType_t credType = SYMMETRIC_PAIR_WISE_KEY;
static OicSecOxm_t oicSecDoxmJustWorks = OIC_JUST_WORKS;
static OicSecOxm_t oicSecDoxmRandomPin = OIC_RANDOM_DEVICE_PIN;
static OicSecDoxm_t defaultDoxm1 =
{
    NULL,                   /* OicUrn_t *oxmType */
    0,                      /* size_t oxmTypeLen */
    &oicSecDoxmJustWorks,  /* uint16_t *oxm */
    1,                      /* size_t oxmLen */
    OIC_JUST_WORKS,         /* uint16_t oxmSel */
    SYMMETRIC_PAIR_WISE_KEY,/* OicSecCredType_t sct */
    false,                  /* bool owned */
    {{0}},            /* OicUuid_t deviceID */
    false,                  /* bool dpc */
    {{0}},            /* OicUuid_t owner */
};

static OicSecDoxm_t defaultDoxm2 =
{
    NULL,                   /* OicUrn_t *oxmType */
    0,                      /* size_t oxmTypeLen */
    &oicSecDoxmRandomPin,  /* uint16_t *oxm */
    1,                      /* size_t oxmLen */
    OIC_RANDOM_DEVICE_PIN,         /* uint16_t oxmSel */
    SYMMETRIC_PAIR_WISE_KEY,/* OicSecCredType_t sct */
    false,                  /* bool owned */
    {{0}},            /* OicUuid_t deviceID */
    false,                  /* bool dpc */
    {{0}},            /* OicUuid_t owner */
};

static void provisioningCB (void* UNUSED1, int UNUSED2, OCProvisionResult_t *UNUSED3, bool UNUSED4)
{
    //dummy callback
    (void) UNUSED1;
    (void) UNUSED2;
    (void) UNUSED3;
    (void) UNUSED4;
}

TEST(OCProvisionPairwiseDevicesTest, NullDevice1)
{
    pDev1.doxm = &defaultDoxm1;
    uint8_t deviceId1[] = {0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x49, 0x64};
    memcpy(pDev1.doxm->deviceID.id, deviceId1, sizeof(deviceId1));

    pDev2.doxm = &defaultDoxm2;
    uint8_t deviceId2[] = {0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x49, 0x63};
    memcpy(pDev2.doxm->deviceID.id, deviceId2, sizeof(deviceId2));

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCProvisionPairwiseDevices(NULL, credType,
                                                              OWNER_PSK_LENGTH_128, NULL, &acl1,
                                                              &pDev2, &acl2, &provisioningCB));
}

TEST(OCProvisionPairwiseDevicesTest, NullDevice2)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCProvisionPairwiseDevices(NULL, credType,
                                                              OWNER_PSK_LENGTH_128, &pDev1, &acl1,
                                                              NULL, &acl2, &provisioningCB));
}

TEST(OCProvisionPairwiseDevicesTest, SamelDeviceId)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCProvisionPairwiseDevices(NULL, credType,
                                                              OWNER_PSK_LENGTH_128, &pDev1, &acl1,
                                                              &pDev1, &acl2, &provisioningCB));
}

TEST(OCProvisionPairwiseDevicesTest, NullCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, OCProvisionPairwiseDevices(NULL, credType,
                                                              OWNER_PSK_LENGTH_128, &pDev1, &acl1,
                                                              &pDev2, &acl2, NULL));
}

TEST(OCProvisionPairwiseDevicesTest, InvalidKeySize)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCProvisionPairwiseDevices(NULL, credType,
                                                              0, &pDev1, &acl1,
                                                              &pDev2, &acl2 ,&provisioningCB));
}

TEST(OCUnlinkDevicesTest, NullDevice1)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCUnlinkDevices(NULL, NULL, &pDev2, provisioningCB));
}

TEST(OCUnlinkDevicesTest, NullDevice2)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCUnlinkDevices(NULL, &pDev1, NULL, provisioningCB));
}

TEST(OCUnlinkDevicesTest, NullCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, OCUnlinkDevices(NULL, &pDev1, &pDev2, NULL));
}

TEST(OCUnlinkDevicesTest, SamelDeviceId)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCUnlinkDevices(NULL,&pDev1, &pDev1, &provisioningCB));
}


TEST(OCRemoveDeviceTest, NullTargetDevice)
{
    unsigned short waitTime = 10 ;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCRemoveDevice(NULL, waitTime, NULL, provisioningCB));
}

TEST(OCRemoveDeviceTest, NullResultCallback)
{
    unsigned short waitTime = 10;
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, OCRemoveDevice(NULL, waitTime, &pDev1, NULL));
}

TEST(OCRemoveDeviceTest, ZeroWaitTime)
{
    unsigned short waitTime = 0;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCRemoveDevice(NULL, waitTime, &pDev1, provisioningCB));
}

TEST(OCGetDevInfoFromNetworkTest, NullUnOwnedDeviceInfo)
{
    unsigned short waitTime = 10;
    OCProvisionDev_t *ownedList = NULL;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCGetDevInfoFromNetwork(waitTime, &ownedList, NULL));
}

TEST(OCGetDevInfoFromNetworkTest, NullOwnedDeviceInfo)
{
    unsigned short waitTime = 10;
    OCProvisionDev_t *unownedList = NULL;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCGetDevInfoFromNetwork(waitTime, NULL, &unownedList));
}

TEST(OCGetDevInfoFromNetworkTest, ZeroWaitTime)
{
    unsigned short waitTime = 0;
    OCProvisionDev_t *ownedList = NULL;
    OCProvisionDev_t *unownedList = NULL;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCGetDevInfoFromNetwork(waitTime, &ownedList, &unownedList));
}

TEST(OCGetLinkedStatusTest, NULLDeviceID)
{
    OCUuidList_t *list = NULL;
    size_t noOfDevices = 0;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCGetLinkedStatus(NULL, &list, &noOfDevices));
}
