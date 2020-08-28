# -*- coding: utf-8 -*-

#mlflow lib
import mlflow
import mlflow.keras
import mlflow.tensorflow

import os
from configparser import ConfigParser
path_config_file = os.getcwd().split('docrv2_sroie')[0] + 'docrv2_sroie/' + 'config.ini'
common_variable = ConfigParser()
common_variable.read(path_config_file)

MLFLOW_URI = common_variable['mlflow']['MLFLOW_URI']
artifact_path = common_variable['mlflow']['artifact_path']

# Enable auto-logging to MLflow to capture TensorBoard metrics.
mlflow.tensorflow.autolog(1)
# mlflow.keras.autolog(1)


def start_mlflow(config):

	mf_experimentname = config.MLFLOW.module_name
	mf_tracking_name = config.MLFLOW.exp_name

	mf_trackinguri = config.MLFLOW.MAIN_URI
	if mf_trackinguri == "":
		mf_trackinguri = MLFLOW_URI

	mlflow.set_tracking_uri(mf_trackinguri)
	try:
		mlflow.create_experiment(mf_experimentname, artifact_location=artifact_path)
	except:
		print('experiment exist')
	mlflow.set_experiment(mf_experimentname)

	mlrun = mlflow.start_run(run_name=mf_tracking_name)

	mlflow.set_tag('config_summary', config.json_string)
	mlflow.log_artifact(config.json_file)
	mlflow.log_params(config.dict)

	config.mlrun = mlrun
	config.MLFLOW.artifact_path = artifact_path

	return config
