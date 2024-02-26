--     Utility programs for working with mysql/mariadb
-- 
--     Copyright (C)  2018 - 2024 Tom Stevelt
-- 
--     This program is free software: you can redistribute it and/or modify
--     it under the terms of the GNU Affero General Public License as
--     published by the Free Software Foundation, either version 3 of the
--     License, or (at your option) any later version.
-- 
--     This program is distributed in the hope that it will be useful,
--     but WITHOUT ANY WARRANTY; without even the implied warranty of
--     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--     GNU Affero General Public License for more details.
-- 
--     You should have received a copy of the GNU Affero General Public License
--     along with this program.  If not, see <https://www.gnu.org/licenses/>.
-- MySQL dump 10.17  Distrib 10.3.17-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: acct
-- ------------------------------------------------------
-- Server version	10.3.17-MariaDB
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
--
-- Table structure for table `account`
--
DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
`acctnum` int(11) NOT NULL DEFAULT 0,
`accttype` char(1) NOT NULL DEFAULT '',
`acctname` char(30) NOT NULL DEFAULT '',
`extranum` char(20) DEFAULT NULL,
`extraline` char(20) DEFAULT NULL,
`openbal` int(11) NOT NULL DEFAULT 0,
`currbal` int(11) NOT NULL DEFAULT 0,
`recondate` date DEFAULT '0000-00-00',
`reconbal` int(11) NOT NULL DEFAULT 0,
`daterange` smallint(6) NOT NULL DEFAULT 3,
PRIMARY KEY (`acctnum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `entcv`
--
DROP TABLE IF EXISTS `entcv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `entcv` (
`acctinv` int(11) NOT NULL,
`acctcv` int(11) NOT NULL,
PRIMARY KEY (`acctinv`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `import`
--
DROP TABLE IF EXISTS `import`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `import` (
`acctnum` int(11) NOT NULL DEFAULT 0,
`website` text DEFAULT NULL,
`login` varchar(20) DEFAULT NULL,
`previous` date NOT NULL DEFAULT '0000-00-00',
`payments` char(1) NOT NULL DEFAULT 'N',
`autoparse` char(1) NOT NULL DEFAULT 'N',
`parsefunc` varchar(20) NOT NULL DEFAULT 'parse',
`headrows` smallint(6) NOT NULL DEFAULT 0,
`colcount` smallint(6) NOT NULL DEFAULT 1,
`refnum` smallint(6) NOT NULL DEFAULT 0,
`date` smallint(6) NOT NULL DEFAULT 0,
`payee` smallint(6) NOT NULL DEFAULT 0,
`increase` smallint(6) NOT NULL DEFAULT 0,
`incsign` char(1) NOT NULL DEFAULT 'N',
`decrease` smallint(6) NOT NULL DEFAULT 0,
`decsign` char(1) NOT NULL DEFAULT 'N',
PRIMARY KEY (`acctnum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `rpt`
--
DROP TABLE IF EXISTS `rpt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rpt` (
`rptname` char(20) NOT NULL DEFAULT '',
`title` char(50) NOT NULL DEFAULT '',
`startdate` date DEFAULT '0000-00-00',
`enddate` date DEFAULT '0000-00-00',
`accttype` char(1) NOT NULL DEFAULT '',
PRIMARY KEY (`rptname`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `rptacct`
--
DROP TABLE IF EXISTS `rptacct`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rptacct` (
`rptname` char(20) NOT NULL DEFAULT '',
`seqnum` smallint(6) NOT NULL DEFAULT 1,
`acctnum` int(11) NOT NULL DEFAULT 0,
KEY `rptndx` (`rptname`,`seqnum`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `system`
--
DROP TABLE IF EXISTS `system`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system` (
`name` char(30) NOT NULL DEFAULT '',
`address` char(30) NOT NULL DEFAULT '',
`city` char(30) NOT NULL DEFAULT '',
`state` char(20) NOT NULL DEFAULT '',
`zipcode` char(20) NOT NULL DEFAULT '',
`phone` char(20) NOT NULL DEFAULT '',
`assetnext` smallint(6) NOT NULL DEFAULT 1001,
`liabnext` smallint(6) NOT NULL DEFAULT 2001,
`equitynext` smallint(6) NOT NULL DEFAULT 3001,
`incomenext` smallint(6) NOT NULL DEFAULT 4001,
`expensenext` smallint(6) NOT NULL DEFAULT 6001,
`trxnext` int(11) NOT NULL DEFAULT 1,
`daterange` smallint(6) NOT NULL DEFAULT 3,
`showamts` char(1) NOT NULL DEFAULT 'Y',
`closedate` date NOT NULL DEFAULT '0000-00-00',
`reconcile` char(1) NOT NULL DEFAULT 'N',
`printdest` char(1) NOT NULL DEFAULT 'S',
`numtrxnew` smallint(6) DEFAULT 1,
`trxpermonth` smallint(6) NOT NULL DEFAULT 100,
`trxstartdate` date NOT NULL DEFAULT '0000-00-00',
`trxstarttrx` int(11) NOT NULL DEFAULT 1,
`trxrollover` smallint(6) NOT NULL DEFAULT 0
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `trxd`
--
DROP TABLE IF EXISTS `trxd`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trxd` (
`trxnum` int(11) NOT NULL DEFAULT 0,
`seqnum` smallint(6) NOT NULL DEFAULT 1,
`acctnum` int(11) NOT NULL DEFAULT 0,
`payee` char(40) DEFAULT NULL,
`amount` int(11) NOT NULL DEFAULT 0,
UNIQUE KEY `trxnum` (`trxnum`,`seqnum`),
KEY `trxdndx` (`acctnum`,`trxnum`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `trxh`
--
DROP TABLE IF EXISTS `trxh`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trxh` (
`trxnum` int(11) NOT NULL DEFAULT 0,
`status` smallint(6) NOT NULL DEFAULT 1,
`refnum` char(8) DEFAULT NULL,
`trxdate` date NOT NULL DEFAULT '0000-00-00',
PRIMARY KEY (`trxnum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
-- Dump completed on 2020-11-11 14:56:41
