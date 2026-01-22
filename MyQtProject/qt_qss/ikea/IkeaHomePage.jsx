import { useState, useEffect } from 'react';
import IkeaStyleHeader from './IkeaStyleHeader';
import IkeaStyleFooter from './IkeaStyleFooter';
import TagCloud from './TagCloud';

import './IkeaHomePage.css';

// Fixed React key warning and export issue

// 日期格式化函数
const formatDate = (dateString, options = {}) => {
  if (!dateString) return '未知日期';
  
  const date = new Date(dateString);
  
  // 检查日期是否有效
  if (isNaN(date.getTime())) {
    return '未知日期';
  }
  
  // 默认选项
  const defaultOptions = {
    year: 'numeric',
    month: 'long',
    day: 'numeric',
    ...options
  };
  
  return date.toLocaleDateString('zh-CN', defaultOptions);
};

const IkeaHomePage = ({ onNavigate }) => {
  const [posts, setPosts] = useState([]);
  const [loading, setLoading] = useState(true);
  const [searchQuery, setSearchQuery] = useState('');
  const [selectedCategory, setSelectedCategory] = useState('all');
  const [selectedTags, setSelectedTags] = useState([]);
  const [categories, setCategories] = useState([]);
  const [tags, setTags] = useState([]);
  const [archives, setArchives] = useState([]);
  const [showArchives, setShowArchives] = useState(false);
  const [hasBingBackground, setHasBingBackground] = useState(false);


  // 检测Bing背景
  useEffect(() => {
    const checkBingBackground = () => {
      const savedUrl = localStorage.getItem('bingWallpaperUrl');
      setHasBingBackground(!!savedUrl);
    };

    // 初始检查
    checkBingBackground();

    // 监听Bing背景变化
    const handleBingWallpaperChange = () => {
      checkBingBackground();
    };

    window.addEventListener('bingWallpaperChanged', handleBingWallpaperChange);
    return () => {
      window.removeEventListener('bingWallpaperChanged', handleBingWallpaperChange);
    };
  }, []);



  // 处理URL参数中的标签
  useEffect(() => {
    const urlParams = new URLSearchParams(window.location.search);
    const tagParam = urlParams.get('tags');
    if (tagParam) {
      const tagsArray = tagParam.split(',').filter(Boolean);
      setSelectedTags(tagsArray);
    }
  }, []);

  // 监听标签云的标签选择事件
  useEffect(() => {
    const handleTagSelected = (event) => {
      const { tagName } = event.detail;
      
      // 切换标签选择状态
      setSelectedTags(prev => {
        const newTags = prev.includes(tagName)
          ? prev.filter(t => t !== tagName)
          : [...prev, tagName];
        
        // 更新URL参数但不刷新页面
        const url = new URL(window.location);
        if (newTags.length > 0) {
          url.searchParams.set('tags', newTags.join(','));
        } else {
          url.searchParams.delete('tags');
        }
        window.history.pushState({}, '', url);
        
        return newTags;
      });
    };

    window.addEventListener('tagSelected', handleTagSelected);
    return () => {
      window.removeEventListener('tagSelected', handleTagSelected);
    };
  }, []);

  // 获取文章数据
  useEffect(() => {
    const fetchPosts = async () => {
      try {
        const response = await fetch('/api/posts?status=published&limit=100');
        if (response.ok) {
          const data = await response.json();
          const postsData = data.posts || [];
          setPosts(postsData);
          
          // 提取分类
          const uniqueCategories = [...new Set(postsData.map(post => post.category).filter(Boolean))];
          setCategories(uniqueCategories);
          
          // 提取标签
          const allTags = postsData.flatMap(post => post.tags || []);
          const uniqueTags = [...new Set(allTags)];
          setTags(uniqueTags);
          
          // 生成归档数据
          const archiveData = generateArchives(postsData);
          setArchives(archiveData);
        } else {
          setPosts([]);
        }
      } catch (error) {
        console.error('获取文章失败:', error);
        setPosts([]);
      } finally {
        setLoading(false);
      }
    };

    const generateArchives = (postsData) => {
      const archiveMap = {};
      postsData.forEach(post => {
        const date = new Date(post.date);
        const year = date.getFullYear();
        const month = date.getMonth() + 1;
        const key = `${year}-${month.toString().padStart(2, '0')}`;
        
        if (!archiveMap[key]) {
          archiveMap[key] = {
            year,
            month,
            monthName: date.toLocaleDateString('zh-CN', { month: 'long' }),
            posts: []
          };
        }
        archiveMap[key].posts.push(post);
      });
      
      return Object.values(archiveMap).sort((a, b) => {
        if (a.year !== b.year) return b.year - a.year;
        return b.month - a.month;
      });
    };

    fetchPosts();
  }, []);

  // 筛选文章
  const filteredPosts = posts.filter(post => {
    const matchesSearch = post.title.toLowerCase().includes(searchQuery.toLowerCase()) ||
                         post.excerpt?.toLowerCase().includes(searchQuery.toLowerCase());
    const matchesCategory = selectedCategory === 'all' || post.category === selectedCategory;
    
    // 多标签筛选：如果选择了标签，文章必须包含所有选中的标签
    const matchesTags = selectedTags.length === 0 || 
                       (post.tags && selectedTags.every(tag => post.tags.includes(tag)));
    
    return matchesSearch && matchesCategory && matchesTags;
  });



  const handleSearch = (query) => {
    setSearchQuery(query);
  };

  const handleCategoryChange = (category) => {
    setSelectedCategory(category);
  };

  const handleTagToggle = (tag) => {
    setSelectedTags(prev => {
      const newTags = prev.includes(tag)
        ? prev.filter(t => t !== tag)
        : [...prev, tag];
      
      // 更新URL参数
      const url = new URL(window.location);
      if (newTags.length > 0) {
        url.searchParams.set('tags', newTags.join(','));
      } else {
        url.searchParams.delete('tags');
      }
      window.history.pushState({}, '', url);
      
      return newTags;
    });
  };

  const clearAllTags = () => {
    setSelectedTags([]);
    const url = new URL(window.location);
    url.searchParams.delete('tags');
    window.history.pushState({}, '', url);
  };

  return (
    <div className={`ikea-home ${hasBingBackground ? 'ikea-home--with-bing-bg' : ''}`}>
      <IkeaStyleHeader 
        currentPage="home" 
        onNavigate={onNavigate}
      />
      
      {/* Hero Section - SpaceX Style */}
      <section className="spacex-hero">
        {/* 动态背景视频/图片 */}
        <div className="spacex-hero-background">
          <div className="spacex-hero-video-overlay"></div>
          {/* 可以替换为实际的背景视频或高质量图片 */}
          <div className="spacex-hero-bg-image"></div>
        </div>
        
        {/* 内容区域 */}
        <div className="spacex-hero-container">
          <div className="spacex-hero-content">
            <h1 className="spacex-hero-title">
              <span className="spacex-hero-title-main">SHIGURE</span>
              <span className="spacex-hero-title-sub">技术探索</span>
            </h1>
            <p className="spacex-hero-subtitle">
              探索前沿技术，分享创新见解，构建数字未来
            </p>
            <div className="spacex-hero-actions">
              <button 
                className="spacex-btn spacex-btn-primary"
                onClick={() => document.getElementById('search-section').scrollIntoView({ behavior: 'smooth' })}
              >
                开始探索
                <svg className="spacex-btn-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor">
                  <path d="M5 12h14M12 5l7 7-7 7"/>
                </svg>
              </button>
              <button 
                className="spacex-btn spacex-btn-secondary"
                onClick={() => onNavigate('about')}
              >
                了解更多
              </button>
            </div>
          </div>
          
          {/* 滚动指示器 */}
          <div className="spacex-scroll-indicator">
            <div className="spacex-scroll-line"></div>
            <div className="spacex-scroll-text">SCROLL</div>
          </div>
        </div>
        
        {/* 粒子效果容器 */}
        <div className="spacex-particles"></div>
      </section>

      {/* Search Section */}
      <section id="search-section" className="ikea-search-section">
        <div className="ikea-container">
          <div className="ikea-search-container">
            <div className="ikea-search-box">
              <input 
                type="text" 
                className="ikea-search-input" 
                placeholder="搜索文章、项目..."
                value={searchQuery}
                onChange={(e) => handleSearch(e.target.value)}
                onKeyPress={(e) => {
                  if (e.key === 'Enter') {
                    handleSearch(e.target.value);
                  }
                }}
              />
              <button 
                className="ikea-search-btn"
                onClick={() => handleSearch(searchQuery)}
              >
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                  <circle cx="11" cy="11" r="8"></circle>
                  <path d="m21 21-4.35-4.35"></path>
                </svg>
              </button>
            </div>
            
            {(selectedTags.length > 0 || selectedCategory !== 'all' || searchQuery) && (
              <div className="ikea-search-results-info">
                找到 {filteredPosts.length} 篇文章
                {selectedCategory !== 'all' && ` · 分类: ${selectedCategory}`}
                {selectedTags.length > 0 && ` · 标签: ${selectedTags.join(', ')}`}
                {searchQuery && ` · 搜索: "${searchQuery}"`}
              </div>
            )}
          </div>
        </div>
      </section>

      {/* Content Section */}
      <section id="content-section" className="ikea-content">
        <div className="ikea-container">
          {/* Section Header */}
          <div className="ikea-section-header">
            <h2 className="ikea-section-title">
              {selectedTags.length > 0 || selectedCategory !== 'all' || searchQuery ? '筛选结果' : '最新文章'}
            </h2>
          </div>

          {/* Filters */}
          <div className="ikea-filters">
            <div className="ikea-filter-group">
              <label className="ikea-filter-label">分类筛选:</label>
              <div className="ikea-filter-buttons">
                {categories.map(category => (
                  <button
                    key={category}
                    className={`ikea-filter-btn ${selectedCategory === category ? 'active' : ''}`}
                    onClick={() => handleCategoryChange(category)}
                  >
                    {category === 'all' ? '全部' : category}
                  </button>
                ))}
              </div>
            </div>

            {tags.length > 0 && (
              <div className="ikea-filter-group">
                <label className="ikea-filter-label">标签筛选:</label>
                <div className="ikea-filter-buttons">
                  <button
                    className={`ikea-filter-btn ${selectedTags.length === 0 ? 'active' : ''}`}
                    onClick={clearAllTags}
                  >
                    清除所有标签
                  </button>
                  {tags.map(tag => (
                    <button
                      key={tag}
                      className={`ikea-filter-btn ikea-filter-btn-tag ${selectedTags.includes(tag) ? 'active' : ''}`}
                      onClick={() => handleTagToggle(tag)}
                    >
                      {tag}
                      {selectedTags.includes(tag) && (
                        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" style={{ marginLeft: '4px' }}>
                          <polyline points="20,6 9,17 4,12"></polyline>
                        </svg>
                      )}
                    </button>
                  ))}
                </div>
              </div>
            )}

            {/* 已选标签显示 */}
            {selectedTags.length > 0 && (
              <div className="ikea-filter-group">
                <label className="ikea-filter-label">已选标签:</label>
                <div className="ikea-selected-tags">
                  {selectedTags.map(tag => (
                    <span key={tag} className="ikea-selected-tag">
                      {tag}
                      <button
                        className="ikea-selected-tag-remove"
                        onClick={() => handleTagToggle(tag)}
                        title="移除标签"
                      >
                        <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                          <line x1="18" y1="6" x2="6" y2="18"></line>
                          <line x1="6" y1="6" x2="18" y2="18"></line>
                        </svg>
                      </button>
                    </span>
                  ))}
                  <span className="ikea-selected-tags-count">
                    ({selectedTags.length} 个标签)
                  </span>
                </div>
              </div>
            )}

            <div className="ikea-filter-group">
              <button
                className={`ikea-filter-btn ${showArchives ? 'active' : ''}`}
                onClick={() => setShowArchives(!showArchives)}
              >
                <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                  <rect x="3" y="4" width="18" height="18" rx="2" ry="2"></rect>
                  <line x1="16" y1="2" x2="16" y2="6"></line>
                  <line x1="8" y1="2" x2="8" y2="6"></line>
                  <line x1="3" y1="10" x2="21" y2="10"></line>
                </svg>
                {showArchives ? '隐藏归档' : '显示归档'}
              </button>

              {/* 清除所有筛选条件按钮 */}
              {(selectedTags.length > 0 || selectedCategory !== 'all' || searchQuery) && (
                <button
                  className="ikea-filter-btn ikea-filter-clear"
                  onClick={() => {
                    setSelectedTags([]);
                    setSelectedCategory('all');
                    setSearchQuery('');
                    const url = new URL(window.location);
                    url.searchParams.delete('tags');
                    window.history.pushState({}, '', url);
                  }}
                >
                  <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                    <path d="M3 6h18M19 6v14c0 1-1 2-2 2H7c-1 0-2-1-2-2V6m3 0V4c0-1 1-2 2-2h4c1 0 2 1 2 2v2"></path>
                  </svg>
                  清除所有筛选
                </button>
              )}
            </div>
          </div>

          {/* Archives */}
          {showArchives && archives.length > 0 && (
            <div className="ikea-archives">
              <h3 className="ikea-archives-title">文章归档</h3>
              <div className="ikea-archives-grid">
                {archives.map(archive => (
                  <div key={`${archive.year}-${archive.month}`} className="ikea-archive-item">
                    <h4 className="ikea-archive-date">
                      {archive.year}年{archive.monthName}
                      <span className="ikea-archive-count">({archive.posts.length}篇)</span>
                    </h4>
                    <ul className="ikea-archive-posts">
                      {archive.posts.map(post => (
                        <li key={post.id} className="ikea-archive-post">
                          <a
                            href={`#blog/${post.id}`}
                            className="ikea-archive-post-link"
                            onClick={(e) => {
                              e.preventDefault();
                              window.location.hash = `#blog/${post.id}`;
                            }}
                          >
                            {post.title}
                          </a>
                          <span className="ikea-archive-post-date">
                            {formatDate(post.date, { month: 'short', day: 'numeric' })}
                          </span>
                        </li>
                      ))}
                    </ul>
                  </div>
                ))}
              </div>
            </div>
          )}

          {/* Posts Grid */}
          {loading ? (
            <div className="ikea-loading">
              <div className="ikea-spinner"></div>
              <p>加载中...</p>
            </div>
          ) : filteredPosts.length > 0 ? (
            <div className="ikea-posts-grid">
              {filteredPosts.map((post, index) => (
                <article key={post._id || post.id || `post-${index}`} className="ikea-post-card">
                  <div className="ikea-post-image">
                    {(post.coverImage || post.image) ? (
                      <img src={post.coverImage || post.image} alt={post.title} />
                    ) : (
                      <div className="ikea-post-placeholder">
                        <svg width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="1">
                          <rect x="3" y="3" width="18" height="18" rx="2" ry="2"/>
                          <circle cx="8.5" cy="8.5" r="1.5"/>
                          <polyline points="21,15 16,10 5,21"/>
                        </svg>
                      </div>
                    )}
                  </div>
                  <div className="ikea-post-content">
                    <div className="ikea-post-meta">
                      <span className="ikea-post-category">{post.category || '未分类'}</span>
                      <span className="ikea-post-date">
                        {formatDate(post.date)}
                      </span>
                    </div>
                    <h3 className="ikea-post-title">{post.title}</h3>
                    <p className="ikea-post-excerpt">
                      {post.excerpt || post.content?.substring(0, 120) + '...'}
                    </p>
                    <button 
                      className="ikea-post-read-more"
                      onClick={() => {
                        console.log('点击阅读全文，文章ID:', post.id);
                        window.location.hash = `#blog/${post.id}`;
                        // 不调用onNavigate，让hash变化自动触发路由
                      }}
                    >
                      阅读全文
                      <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <path d="M5 12h14M12 5l7 7-7 7"/>
                      </svg>
                    </button>
                  </div>
                </article>
              ))}
            </div>
          ) : (
            <div className="ikea-empty-state">
              <svg width="64" height="64" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="1">
                <circle cx="11" cy="11" r="8"/>
                <path d="m21 21-4.35-4.35"/>
              </svg>
              <h3>没有找到相关文章</h3>
              <p>尝试调整搜索条件或浏览其他分类</p>
            </div>
          )}
        </div>
      </section>

      {/* Tag Cloud Section */}
      <section className="ikea-tag-cloud-section">
        <div className="ikea-container">
          <TagCloud />
        </div>
      </section>

      <IkeaStyleFooter onNavigate={onNavigate} />
    </div>
  );
};

export default IkeaHomePage;