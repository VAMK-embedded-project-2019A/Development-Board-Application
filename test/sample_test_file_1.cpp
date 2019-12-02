#include <gtest/gtest.h>

#include <servercomm.h>

TEST(SAMPLE_1, sample_1_1)
{
	ServerComm server_comm;
	EXPECT_EQ(std::string{}, server_comm.getSongName());
}