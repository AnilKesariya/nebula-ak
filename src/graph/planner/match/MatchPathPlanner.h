// Copyright (c) 2022 vesoft inc. All rights reserved.
//
// This source code is licensed under Apache 2.0 License.

#pragma once

#include "graph/planner/match/CypherClausePlanner.h"

namespace nebula {
namespace graph {
// The MatchPathPlanner generates plan for match clause;
class MatchPathPlanner final {
 public:
  MatchPathPlanner() = default;

  StatusOr<SubPlan> transform(QueryContext* qctx,
                              GraphSpaceID spaceId,
                              Expression* bindFilter,
                              const std::unordered_map<std::string, AliasType>& aliasesAvailable,
                              std::unordered_set<std::string> nodeAliasesSeen,
                              Path& path);

 private:
  Status findStarts(std::vector<NodeInfo>& nodeInfos,
                    std::vector<EdgeInfo>& edgeInfos,
                    QueryContext* qctx,
                    GraphSpaceID spaceId,
                    Expression* bindFilter,
                    const std::unordered_map<std::string, AliasType>& aliasesAvailable,
                    std::unordered_set<std::string> nodeAliases,
                    bool& startFromEdge,
                    size_t& startIndex,
                    SubPlan& matchClausePlan);

  Status expand(const std::vector<NodeInfo>& nodeInfos,
                const std::vector<EdgeInfo>& edgeInfos,
                QueryContext* qctx,
                GraphSpaceID spaceId,
                bool startFromEdge,
                size_t startIndex,
                SubPlan& subplan,
                std::unordered_set<std::string>& nodeAliasesSeenInPattern);

  Status expandFromNode(const std::vector<NodeInfo>& nodeInfos,
                        const std::vector<EdgeInfo>& edgeInfos,
                        QueryContext* qctx,
                        GraphSpaceID spaceId,
                        size_t startIndex,
                        SubPlan& subplan,
                        std::unordered_set<std::string>& nodeAliasesSeenInPattern);

  PlanNode* joinLeftAndRightExpandPart(QueryContext* qctx, PlanNode* left, PlanNode* right);

  Status leftExpandFromNode(const std::vector<NodeInfo>& nodeInfos,
                            const std::vector<EdgeInfo>& edgeInfos,
                            QueryContext* qctx,
                            GraphSpaceID spaceId,
                            size_t startIndex,
                            std::string inputVar,
                            SubPlan& subplan,
                            std::unordered_set<std::string>& nodeAliasesSeenInPattern);

  Status rightExpandFromNode(const std::vector<NodeInfo>& nodeInfos,
                             const std::vector<EdgeInfo>& edgeInfos,
                             QueryContext* qctx,
                             GraphSpaceID spaceId,
                             size_t startIndex,
                             SubPlan& subplan,
                             std::unordered_set<std::string>& nodeAliasesSeenInPattern);

  Status expandFromEdge(const std::vector<NodeInfo>& nodeInfos,
                        const std::vector<EdgeInfo>& edgeInfos,
                        QueryContext* qctx,
                        GraphSpaceID spaceId,
                        size_t startIndex,
                        SubPlan& subplan,
                        std::unordered_set<std::string>& nodeAliasesSeenInPattern);

  // Project all named alias.
  // TODO: Might not neccessary
  Status projectColumnsBySymbols(QueryContext* qctx, Path& path, SubPlan& plan);

  YieldColumn* buildVertexColumn(ObjectPool* pool, const std::string& alias) const;

  YieldColumn* buildEdgeColumn(ObjectPool* pool, EdgeInfo& edge) const;

  YieldColumn* buildPathColumn(Expression* pathBuild, const std::string& alias) const;

 private:
  Expression* initialExpr_{nullptr};
};
}  // namespace graph
}  // namespace nebula
