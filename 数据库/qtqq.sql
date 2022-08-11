/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 80020
Source Host           : localhost:3306
Source Database       : qtqq

Target Server Type    : MYSQL
Target Server Version : 80020
File Encoding         : 65001

Date: 2022-08-11 20:34:19
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `tab_accounts`
-- ----------------------------
DROP TABLE IF EXISTS `tab_accounts`;
CREATE TABLE `tab_accounts` (
  `employeeID` int NOT NULL COMMENT '员工工号',
  `account` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '账号',
  `code` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '密码',
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tab_accounts
-- ----------------------------
INSERT INTO `tab_accounts` VALUES ('10002', 'rener', 'rener');
INSERT INTO `tab_accounts` VALUES ('10003', 'rensan', 'rensan');
INSERT INTO `tab_accounts` VALUES ('10001', 'renyi', 'renyi');
INSERT INTO `tab_accounts` VALUES ('10008', 'shier', 'shier');
INSERT INTO `tab_accounts` VALUES ('10009', 'shisan', 'shisan');
INSERT INTO `tab_accounts` VALUES ('10007', 'shiyi', 'shiyi');
INSERT INTO `tab_accounts` VALUES ('10005', 'yaner', 'yaner');
INSERT INTO `tab_accounts` VALUES ('10006', 'yansan', 'yansan');
INSERT INTO `tab_accounts` VALUES ('10004', 'yanyi', 'yanyi');

-- ----------------------------
-- Table structure for `tab_department`
-- ----------------------------
DROP TABLE IF EXISTS `tab_department`;
CREATE TABLE `tab_department` (
  `departmentID` int NOT NULL AUTO_INCREMENT COMMENT '鍏徃閮ㄩ棬鍙?閮ㄩ棬缇ゅ彿)',
  `department_name` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '部门号',
  `picture` char(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '部门头像路径',
  `sign` char(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '部门口号',
  PRIMARY KEY (`departmentID`)
) ENGINE=InnoDB AUTO_INCREMENT=2004 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tab_department
-- ----------------------------
INSERT INTO `tab_department` VALUES ('2000', '公司群', 'D:\\qtqq_images\\gsq.png', '越分享越拥有');
INSERT INTO `tab_department` VALUES ('2001', '人事部', 'D:\\qtqq_images\\rsb.png', 'java/c++');
INSERT INTO `tab_department` VALUES ('2002', '研发部', 'D:\\qtqq_images\\yfb.png', '世界只有两种编程语言:一种没兴趣,一种生活所迫');
INSERT INTO `tab_department` VALUES ('2003', '市场部', 'D:\\qtqq_images\\scb.png', '下个月出去找工作');

-- ----------------------------
-- Table structure for `tab_employees`
-- ----------------------------
DROP TABLE IF EXISTS `tab_employees`;
CREATE TABLE `tab_employees` (
  `departmentID` int NOT NULL COMMENT '群ID(群号)',
  `employeeID` int NOT NULL AUTO_INCREMENT COMMENT '宸ュ彿(鍛樺伐QQ鍙?',
  `employee_name` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '员工姓名',
  `employee_sign` char(200) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '员工签名',
  `status` tinyint NOT NULL DEFAULT '1' COMMENT '1有效 0注销',
  `picture` char(150) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '鍛樺伐澶村儚鐨勮矾寰?',
  `online` tinyint NOT NULL DEFAULT '1' COMMENT '鍦ㄧ嚎鐘舵€?1绂荤嚎 2鍦ㄧ嚎',
  PRIMARY KEY (`employeeID`)
) ENGINE=InnoDB AUTO_INCREMENT=10012 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tab_employees
-- ----------------------------
INSERT INTO `tab_employees` VALUES ('2001', '10001', 'renyi', '写点什么呢', '1', 'D:\\qtqq_images\\r1.png', '1');
INSERT INTO `tab_employees` VALUES ('2001', '10002', 'rener', '男主', '1', 'D:\\qtqq_images\\r2.png', '0');
INSERT INTO `tab_employees` VALUES ('2001', '10003', 'rensan', '女主', '1', 'D:\\qtqq_images\\r3.png', '0');
INSERT INTO `tab_employees` VALUES ('2002', '10004', 'yanyi', '女主她妈', '1', 'D:\\qtqq_images\\y1.png', '1');
INSERT INTO `tab_employees` VALUES ('2002', '10005', 'yaner', 'goddess', '1', 'D:\\qtqq_images\\y2.png', '0');
INSERT INTO `tab_employees` VALUES ('2002', '10006', 'yansan', '开创中', '1', 'D:\\qtqq_images\\y3.png', '0');
INSERT INTO `tab_employees` VALUES ('2003', '10007', 'shiyi', '去国外中', '1', 'D:\\qtqq_images\\s1.png', '0');
INSERT INTO `tab_employees` VALUES ('2003', '10008', 'shier', '拍电视剧中', '1', 'D:\\qtqq_images\\s2.png', '0');
INSERT INTO `tab_employees` VALUES ('2003', '10009', 'shisan', '商讨后事中', '1', 'D:\\qtqq_images\\s3.png', '0');
INSERT INTO `tab_employees` VALUES ('2001', '10010', '小白', null, '1', 'D:\\qt_workspace\\QtQQ_Server\\QtQQ_Server\\qiniu.jpg', '0');
INSERT INTO `tab_employees` VALUES ('2002', '10011', '大白', null, '1', 'D:\\qt_workspace\\QtQQ_Server\\QtQQ_Server\\qiniu.jpg', '0');
