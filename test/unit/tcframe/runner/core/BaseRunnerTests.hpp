#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../../os/MockOperatingSystem.hpp"
#include "../../driver/MockDriver.hpp"
#include "../../runner/aggregator/MockAggregator.hpp"
#include "../../runner/aggregator/MockAggregatorRegistry.hpp"
#include "../../runner/evaluator/MockEvaluator.hpp"
#include "../../runner/evaluator/MockEvaluatorRegistry.hpp"
#include "../../runner/generator/MockGenerator.hpp"
#include "../../runner/grader/MockGrader.hpp"
#include "../../runner/grader/MockGraderLogger.hpp"
#include "../../runner/grader/MockGraderLoggerFactory.hpp"
#include "../logger/MockRunnerLogger.hpp"
#include "tcframe/runner/core/Runner.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Ne;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;
using ::testing::Truly;

namespace tcframe {

class BaseRunnerTests : public Test {
protected:
    class ProblemSpec : public BaseProblemSpec {
    protected:
        void InputFormat() {}
    };

    string specPath = "/Users/fushar/january contest/c_slug/spec.cpp";

    int argc = 1;
    char** argv =  new char*[1]{(char*) "./runner"};

    LoggerEngine* loggerEngine = new SimpleLoggerEngine();

    MOCK(RunnerLogger) runnerLogger;
    MOCK(GraderLogger) graderLogger;
    MOCK(Evaluator) evaluator;
    MOCK(Aggregator) aggregator;
    MOCK(Generator) generator;
    MOCK(Grader) grader;

    MOCK(OperatingSystem) os;
    MOCK(RunnerLoggerFactory) runnerLoggerFactory;
    MOCK(GraderLoggerFactory) graderLoggerFactory;
    MOCK(GeneratorFactory) generatorFactory;
    MOCK(GraderFactory) graderFactory;
    MOCK(EvaluatorRegistry) evaluatorRegistry;
    MOCK(AggregatorRegistry) aggregatorRegistry;

    void SetUp() {
        ON_CALL(runnerLoggerFactory, create(_)).WillByDefault(Return(&runnerLogger));
        ON_CALL(graderLoggerFactory, create(_, _)).WillByDefault(Return(&graderLogger));
        ON_CALL(generatorFactory, create(_, _, _, _, _)).WillByDefault(Return(&generator));
        ON_CALL(graderFactory, create(_, _, _, _)).WillByDefault(Return(&grader));
        ON_CALL(evaluatorRegistry, get(_, _, _)).WillByDefault(Return(&evaluator));
        ON_CALL(aggregatorRegistry, get(_)).WillByDefault(Return(&aggregator));
        ON_CALL(os, execute(_)).WillByDefault(Return(ExecutionResult()));
    }

    template<typename TProblemSpec>
    Runner<TProblemSpec> createRunner(BaseTestSpec<TProblemSpec>* testSpec) {
        auto driver = new Driver<TProblemSpec>(specPath, testSpec);

        return Runner<TProblemSpec>(
                driver, loggerEngine, &os,&runnerLoggerFactory, &graderLoggerFactory,
                &generatorFactory, &graderFactory, &evaluatorRegistry, &aggregatorRegistry);
    }

    struct HelperKeyIs {
        string key_;
        string value_;

        HelperKeyIs(const string& key, const string& value)
                : key_(key)
                , value_(value) {}

        bool operator()(const map<string, string>& m) const {
            if (value_.empty()) {
                return !m.count(key_);
            }
            return m.count(key_) && m.at(key_) == value_;
        }
    };
};

}
