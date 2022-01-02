//
// Created by ounol on 2021-10-25.
//

#include <Util/AssetsDef.h>
#include <Util/SafeLog.h>
#include <Manager/AssetMgr.h>
#include "MapDataUtil.h"

using namespace CSE;

std::vector<std::vector<MapDataUtil::NodeState>> MapDataUtil::m_mapNode = std::vector<std::vector<NodeState>>();
ivec2 MapDataUtil::m_size = ivec2{0, 0};
ivec2 MapDataUtil::m_center = ivec2{0, 0};

COMPONENT_CONSTRUCTOR(MapDataUtil) {

}

MapDataUtil::~MapDataUtil() = default;

void MapDataUtil::InitMapData() {
    auto map_raw = CSE::AssetMgr::LoadAssetFile(CSE::AssetsPath() + "map_tile.txt");
    auto map_raw_split = CSE::split(map_raw, '\n');



    for (auto y = 0; y < map_raw_split.size(); ++y) {
        const auto& str_line = map_raw_split[y];
        if(m_size.x <= 0) {
            m_size.x = str_line.size() - 1;
            m_size.y = map_raw_split.size();

            m_mapNode.resize(m_size.x);
            for (auto& row : m_mapNode) {
                row.resize(m_size.y);
            }
        }

        for (int x = 0; x < m_size.x; ++x) {
            const auto& str = str_line[x];
            int value = str - '0';
            m_mapNode[x][y] = (NodeState)value;
        }
    }

    m_center = ivec2{ m_size.x / 2, m_size.y / 2 };
}

void MapDataUtil::Init() {

}

void MapDataUtil::Tick(float elapsedTime) {

}

void MapDataUtil::Exterminate() {

}

CSE::ivec2 MapDataUtil::GetIndexToPosition(const vec3& position) {
    auto position_correction = vec2{ position.x + (float) m_center.x * s_space,
                                     position.z + (float) m_center.y * s_space };
    return ivec2{ static_cast<int>(round(position_correction.x / s_space)),
                  static_cast<int>(round(position_correction.y / s_space)) };
}

CSE::vec3 MapDataUtil::GetPositionToIndex(const ivec2& index) {
    auto index_correction = vec3{ static_cast<float>(index.x - m_center.x), 0, static_cast<float>(index.y - m_center.y)};
    return index_correction * s_space;
}

std::list<Direction> MapDataUtil::GetTilePath(const ivec2& index, bool inboxIgnore) {
    auto&& result = std::list<Direction>();

    const auto& node = m_mapNode[index.x][index.y];
    const auto wallIndex = inboxIgnore ? 9 : 8;

    if (node == WALL || (!inboxIgnore && node == INBOX_WALL))
        return result;

    {   // Left
        auto nextIndex = index + ivec2{ -1, 0 };
        if(nextIndex.x >= 0 && m_mapNode[nextIndex.x][nextIndex.y] < wallIndex)
            result.push_back(LEFT);
    }
    {   // Right
        auto nextIndex = index + ivec2{ 1, 0 };
        if(nextIndex.x <= m_size.x - 1 && m_mapNode[nextIndex.x][nextIndex.y] < wallIndex)
            result.push_back(RIGHT);
    }
    {   // Up
        auto nextIndex = index + ivec2{ 0, -1 };
        if(nextIndex.y >= 0 && m_mapNode[nextIndex.x][nextIndex.y] < wallIndex)
            result.push_back(UP);
    }
    {   // Down
        auto nextIndex = index + ivec2{ 0, 1 };
        if(nextIndex.y <= m_size.y - 1 && m_mapNode[nextIndex.x][nextIndex.y] < wallIndex)
            result.push_back(DOWN);
    }

    return result;
}

std::list<CSE::ivec2> MapDataUtil::PathFinding(CSE::ivec2 start, CSE::ivec2 end) {
    auto result = std::list<CSE::ivec2>();
    auto history = std::list<CSE::ivec2>();
    SetPathToFind(result, history, start, start, end);
    std::reverse(result.begin(), result.end());
    return result;
}

bool
MapDataUtil::SetPathToFind(std::list<CSE::ivec2>& src, std::list<CSE::ivec2>& history, CSE::ivec2 current,
                           CSE::ivec2 start, CSE::ivec2 end) {
    auto tilePathList = GetTilePath(current, true);
    if (tilePathList.empty()) return false;

    // 파인더 뇌절 방지 선
    if(history.size() > std::abs(start.x - end.x) + std::abs(start.y - end.y) + 10) {
        return false;
    }

    struct ValueStruct {
        float value;
        ivec2 nextIndex;
    };

    auto valueList = std::vector<ValueStruct>();
    valueList.reserve(tilePathList.size());

    for (const Direction direction : tilePathList) {
        auto nextIndex = ivec2();
        switch (direction) {
            case ::NONE:
                return false;
            case LEFT:
                nextIndex = { current.x - 1, current.y };
                break;
            case RIGHT:
                nextIndex = { current.x + 1, current.y };
                break;
            case UP:
                nextIndex = { current.x, current.y - 1 };
                break;
            case DOWN:
                nextIndex = { current.x, current.y + 1 };
                break;
        }
        if (nextIndex.x == end.x && nextIndex.y == end.y) {
            src.emplace_back(end.x, end.y);
            src.emplace_back(current.x, current.y);
            return true;
        }
        valueList.push_back(
                ValueStruct{
                        static_cast<float>(history.size() + ivec2::Distance(nextIndex, end)),
                    nextIndex
                });
    }

    std::sort(valueList.begin(), valueList.end(), []
    (ValueStruct first, ValueStruct second) -> bool {
        return first.value < second.value;
    });

    for (const auto& value : valueList) {
        bool isNewPath = true;
        for (const auto& history_path : history) {
            if(current.x == history_path.x && current.y == history_path.y) {
                isNewPath = false;
                break;
            }
        }
        if(!isNewPath) continue;
        history.push_back(current);
        if(SetPathToFind(src, history, value.nextIndex, start, end)) {
            src.emplace_back(current.x, current.y);
            return true;
        }
        history.pop_back();
    }
    return false;
}
