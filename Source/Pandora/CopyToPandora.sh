#!/bin/bash

rm -f .pandora_config
touch .pandora_config

if [ -z "${PANDORA_USER}" ]; then
	read -ep "Pandora User: " PANDORA_USER
	echo "export PANDORA_USER=${PANDORA_USER}" >> .pandora_config
fi

if [ -z "${PANDORA_IP}" ]; then
	read -ep "Pandora IP Address: " PANDORA_IP
	echo "export PANDORA_IP=${PANDORA_IP}" >> .pandora_config
fi

if [ -z "${PANDORA_DIRECTORY}" ]; then
	read -ep "Pandora Directory: " PANDORA_DIRECTORY
	echo "export PANDORA_DIRECTORY='${PANDORA_DIRECTORY}'" >> .pandora_config
fi

scp ../../Bin/Pandora/arm/32/* ${PANDORA_USER}@${PANDORA_IP}:${PANDORA_DIRECTORY}

if [[ $? != 0 ]]; then
	echo "Failed to copy the files to ${PANDORA_IP}:'${PANDORA_DIRECTORY}' for user: ${PANDORA_USER}"
	exit 1
fi

if [[ -s .pandora_config ]]; then
	echo "You may want to run:"
	echo ". .pandora_config"
	echo "to save your settings, or copy the contents to your ${HOME}/.bashrc file"
fi
