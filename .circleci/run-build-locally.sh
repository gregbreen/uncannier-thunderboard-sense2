curl --user ${CIRCLE_TOKEN}: \
    --request POST \
    --form revision=$1\
    --form config=@config.yml \
    --form notify=false \
        https://circleci.com/api/v1.1/project/github/gregbreen/uncannier-thunderboard-sense2/tree/$2
