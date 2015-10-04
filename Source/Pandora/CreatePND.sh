#!/bin/bash

export TAG_NAME=`git describe --tags | sed -e 's/_[0-9].*//'`
export VERSION_NUM=`git describe --match "${TAG_NAME}_[0-9]*" HEAD | sed -e 's/-g.*//' -e "s/${TAG_NAME}_//"`
export MAJOR=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.[^.]*$//' | sed -r 's/.[^.]*$//'`
export MINOR=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.[^.]*$//' | sed -r 's/.[.]*//'`
export REVISION=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.*(.[0-9].)//'`
export BUILD=`echo $VERSION_NUM | sed -e 's/[0-9].[0-9].[0-9]//' -e 's/-//'`

export ROOTDIR=$(cd ../../ && pwd)

mkdir -p ${ROOTDIR}/Build/PND
cp -rv ${ROOTDIR}/Bin/Pandora/arm/32/Aura ${ROOTDIR}/Build/PND/

cat > ${ROOTDIR}/Build/PND/Aura_PXML.xml << __EOF__
<?xml version="1.0" encoding="UTF-8"?>
<PXML xmlns="http://openpandora.org/namespaces/PXML">
	<package id="Rico.Aura">
		<version major="${MAJOR}" minor="${MINOR}" release="${REVISION}" build="${BUILD}"/>
		<author name="Rico Tyrell" email="aura@redringrico.com"/>
		<titles>
			<title lang="en_US">Aura</title>
			<title lang="en_GB">Aura</title>
		</titles>
	</package>
	<application id="Rico.Aura">
		<titles>
			<title lang="en_US">Aura</title>
			<title lang="en_GB">Aura</title>
		</titles>
		<version major="${MAJOR}" minor="${MINOR}" release="${REVISION}" build="${BUILD}"/>
		<descriptions>
			<description lang="en_US">A cyberpunk-themed first person shooter</description>
			<description lang="en_GB">A cyberpunk-themed first person shooter</description>
		</descriptions>
		<exec command="RunAura.sh"/>
		<author name="Rico Tyrell" email="aura@redringrico.com"/>
		<categories>
			<category name="Game">
				<subcategory name="ActionGame"/>
			</category>
		</categories>
		<licenses>
			<license name="GPLv3" url="http://www.gnu.org/licenses/gpl.html" sourcecodeurl="https://www.github.com/RedRingRico/Aura"/>
		</licenses>
	</application>
</PXML>
__EOF__

cat > ${ROOTDIR}/Build/PND/RunAura.sh << __EOF__
cat /proc/pandora/nub0/mode > /tmp/nub0mode_old
cat /proc/pandora/nub1/mode > /tmp/nub1mode_old
/usr/pandora/scripts/op_nubchange.sh absolute absolute

./Aura

nub0mode=`cat /tmp/nub0mode_old`
nub1mode=`cat /tmp/nub1mode_old`
/usr/pandora/scripts/op_nubchange.sh nub0mode nub1mode
rm /tmp/nub0mode_old /tmp/nub1mode_old
__EOF__ 

chmod 755 ${ROOTDIR}/Build/PND/RunAura.sh

cat > ${ROOTDIR}/Build/PND/powervr.ini << __EOF__
[default]
WindowSystem=libpvrPVR2D_FLIPWSEGL.so
__EOF__

cp -rv ${ROOTDIR}/Content/* ${ROOTDIR}/Build/PND/

cp --preserve=links -rv $PANDORA_SDK/toolchain/arm-none-linux-gnueabi/lib/libstdc++.*so* ${ROOTDIR}/Build/PND/
rm -rv ${ROOTDIR}/Build/PND/libstdc++.so.6.0.18-gdb.py

./pnd_make.sh -d ${ROOTDIR}/Build/PND -p ${ROOTDIR}/Build/aura.pnd -x ${ROOTDIR}/Build/PND/Aura_PXML.xml

