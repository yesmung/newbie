# -*- coding: utf-8 -*-

# mlflow lib
import mlflow
import mlflow.tensorflow

MLFLOW_URI = 'sqlite:////media/myungsungkwak/msdisk/mlflow_data/mlflow.db'
artifact_path = '/media/myungsungkwak/msdisk/MLFLOW_SQLITE/'

# Enable auto-logging to MLflow to capture TensorBoard metrics.
mlflow.tensorflow.autolog(1)

def start_mlflow(config):
    # try:
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