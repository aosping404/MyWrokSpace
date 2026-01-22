import IkeaStyleHeader from './IkeaStyleHeader';
import IkeaStyleFooter from './IkeaStyleFooter';
import './IkeaAboutPage.css';

const IkeaAboutPage = ({ onNavigate }) => {
  return (
    <div className="ikea-about">
      <IkeaStyleHeader 
        currentPage="about" 
        onNavigate={onNavigate}
      />
      
      {/* Hero Section */}
      <section className="ikea-about-hero">
        <div className="ikea-container">
          <div className="ikea-about-hero-content">
            <h1 className="ikea-about-title">关于我</h1>
            <p className="ikea-about-subtitle">
              一个热爱技术、喜欢分享的开发者
            </p>
          </div>
        </div>
      </section>

      {/* Main Content */}
      <section className="ikea-about-content">
        <div className="ikea-container">
          <div className="ikea-about-grid">
            {/* 个人介绍 */}
            <div className="ikea-about-section">
              <div className="ikea-about-card">
                <div className="ikea-about-card-header">
                  <div className="ikea-about-avatar">
                    <img src="/img/logo_simple_white.png" alt="Shigure" />
                  </div>
                  <div className="ikea-about-info">
                    <h2>Shigure</h2>
                    <p>全栈开发工程师</p>
                  </div>
                </div>
                <div className="ikea-about-card-content">
                  <p>
                    你好！我是Shigure，一名热爱技术的全栈开发工程师。我专注于现代Web开发技术，
                    包括React、Node.js、TypeScript等前沿技术栈。
                  </p>
                  <p>
                    在这个博客中，我会分享我的技术学习心得、项目开发经验，以及一些有趣的ACG内容。
                    希望能够通过分享，与更多志同道合的朋友交流学习。
                  </p>
                </div>
              </div>
            </div>

            {/* 技能栈 */}
            <div className="ikea-about-section">
              <h3 className="ikea-section-title">技术栈</h3>
              <div className="ikea-skills-grid">
                <div className="ikea-skill-category">
                  <h4>前端开发</h4>
                  <div className="ikea-skill-tags">
                    <span className="ikea-skill-tag">React</span>
                    <span className="ikea-skill-tag">Vue.js</span>
                    <span className="ikea-skill-tag">TypeScript</span>
                    <span className="ikea-skill-tag">JavaScript</span>
                    <span className="ikea-skill-tag">HTML5</span>
                    <span className="ikea-skill-tag">CSS3</span>
                  </div>
                </div>
                <div className="ikea-skill-category">
                  <h4>后端开发</h4>
                  <div className="ikea-skill-tags">
                    <span className="ikea-skill-tag">Node.js</span>
                    <span className="ikea-skill-tag">Express</span>
                    <span className="ikea-skill-tag">MongoDB</span>
                    <span className="ikea-skill-tag">MySQL</span>
                    <span className="ikea-skill-tag">Redis</span>
                  </div>
                </div>
                <div className="ikea-skill-category">
                  <h4>工具与部署</h4>
                  <div className="ikea-skill-tags">
                    <span className="ikea-skill-tag">Docker</span>
                    <span className="ikea-skill-tag">Git</span>
                    <span className="ikea-skill-tag">Webpack</span>
                    <span className="ikea-skill-tag">Vite</span>
                    <span className="ikea-skill-tag">Linux</span>
                  </div>
                </div>
              </div>
            </div>

            {/* 兴趣爱好 */}
            <div className="ikea-about-section">
              <h3 className="ikea-section-title">兴趣爱好</h3>
              <div className="ikea-interests-grid">
                <div className="ikea-interest-card">
                  <div className="ikea-interest-icon">
                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                      <rect x="2" y="3" width="20" height="14" rx="2" ry="2"/>
                      <line x1="8" y1="21" x2="16" y2="21"/>
                      <line x1="12" y1="17" x2="12" y2="21"/>
                    </svg>
                  </div>
                  <h4>编程开发</h4>
                  <p>热爱探索新技术，享受解决问题的过程</p>
                </div>
                <div className="ikea-interest-card">
                  <div className="ikea-interest-icon">
                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                      <path d="M14.7 6.3a1 1 0 0 0 0 1.4l1.6 1.6a1 1 0 0 0 1.4 0l3.77-3.77a6 6 0 0 1-7.94 7.94l-6.91 6.91a2.12 2.12 0 0 1-3-3l6.91-6.91a6 6 0 0 1 7.94-7.94l-3.76 3.76z"/>
                    </svg>
                  </div>
                  <h4>ACG文化</h4>
                  <p>动画、漫画、游戏的忠实爱好者</p>
                </div>
                <div className="ikea-interest-card">
                  <div className="ikea-interest-icon">
                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                      <path d="M12 2l3.09 6.26L22 9.27l-5 4.87 1.18 6.88L12 17.77l-6.18 3.25L7 14.14 2 9.27l6.91-1.01L12 2z"/>
                    </svg>
                  </div>
                  <h4>知识分享</h4>
                  <p>通过博客和开源项目分享经验</p>
                </div>
              </div>
            </div>

            {/* 联系方式 */}
            <div className="ikea-about-section">
              <h3 className="ikea-section-title">联系我</h3>
              <div className="ikea-contact-grid">
                <a 
                  href="https://github.com/Shigure-moon" 
                  target="_blank" 
                  rel="noopener noreferrer"
                  className="ikea-contact-card"
                >
                  <div className="ikea-contact-icon">
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor">
                      <path d="M12 0c-6.626 0-12 5.373-12 12 0 5.302 3.438 9.8 8.207 11.387.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23.957-.266 1.983-.399 3.003-.404 1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576 4.765-1.589 8.199-6.086 8.199-11.386 0-6.627-5.373-12-12-12z"/>
                    </svg>
                  </div>
                  <div className="ikea-contact-info">
                    <h4>GitHub</h4>
                    <p>查看我的开源项目</p>
                  </div>
                </a>
                <a 
                  href="mailto:a993056494@163.com" 
                  className="ikea-contact-card"
                >
                  <div className="ikea-contact-icon">
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                      <path d="M4 4h16c1.1 0 2 .9 2 2v12c0 1.1-.9 2-2 2H4c-1.1 0-2-.9-2-2V6c0-1.1.9-2 2-2z"/>
                      <polyline points="22,6 12,13 2,6"/>
                    </svg>
                  </div>
                  <div className="ikea-contact-info">
                    <h4>邮箱</h4>
                    <p>a993056494@163.com</p>
                  </div>
                </a>
                <button 
                  className="ikea-contact-card"
                  onClick={() => onNavigate('contact')}
                >
                  <div className="ikea-contact-icon">
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                      <path d="M21 15a2 2 0 0 1-2 2H7l-4 4V5a2 2 0 0 1 2-2h14a2 2 0 0 1 2 2z"/>
                    </svg>
                  </div>
                  <div className="ikea-contact-info">
                    <h4>留言</h4>
                    <p>通过联系页面给我留言</p>
                  </div>
                </button>
              </div>
            </div>
          </div>
        </div>
      </section>

      <IkeaStyleFooter onNavigate={onNavigate} />
    </div>
  );
};

export default IkeaAboutPage;